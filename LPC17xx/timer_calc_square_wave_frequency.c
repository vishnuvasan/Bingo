/**
 * @file	: timer_calc_square_wave_frequency.c
 * @purpose	: A simple driver to calculate the frequency of a square wave using Capture in Timer 0 and Timer 1
 * @version	: 1.0
 * @date	: 17. May. 2014
 * @author	: Vishnu Vasan Nehru
 *----------------------------------------------------------------------------
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_nvic.h"
#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"


#define LED_PIN 	(1<<6)
#define LED2_MASK	((1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6))
#define LED1_MASK	((1<<29) | (1<<31))
#define MAX_MATCH	1000000
#define START_MATCH	10000


/************************** PRIVATE TYPES *************************/

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"Timer Match interrupt demo \n\r"
"\t - MCU: LPC17xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART0 -  115200 bps \n\r"
" Use timer 0 and timer 1 to calculate the frequency of square wave \n\r"
" on CAP0.0 transitions \n\r"
"********************************************************************************\n\r";

// UART Configuration structure variable
UART_CFG_Type UARTConfigStruct;

//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;

/* Initialize Two Timer Configurations Timer0 and Timer 1 */
TIM_CAPTURECFG_Type TIM_CaptureConfigStruct,Tim_CaptureConfigStruct1;

uint8_t volatile timer0_flag = FALSE, timer1_flag = FALSE;
/************************** PRIVATE FUNCTION *************************/
void print_menu(void);
void TIMER0_IRQHandler(void);
void TIMER1_IRQHandler(void);
// Print Menu 1
void print_menu(void)
{
	_DBG(menu1);
}

void TIMER0_IRQHandler(void)
{
	if (TIM_GetIntCaptureStatus(LPC_TIM0,0))
	{
		TIM_ClearIntCapturePending(LPC_TIM0,0);
		_DBG("Time capture: ");
		_DBH32(TIM_GetCaptureValue(LPC_TIM0,0));_DBG_("");
	}
}
void TIMER1_IRQHandler(void)
{
	if (TIM_GetIntCaptureStatus(LPC_TIM1,0))
	{
		TIM_ClearIntCapturePending(LPC_TIM1,0);
		_DBG("Time capture: ");
		_DBH32(TIM_GetCaptureValue(LPC_TIM1,0));
		CALC_Freq();
		_DBG_("");
	}
}


void CALC_Freq(void)
{
	uint32_t Timer0_Value,Timer1_Value,Timer_Interim;
	
	Timer0_Value=TIM_GetCaptureValue(LPC_TIM0,1);
	_DBG("Timer 0 Capture : ");
	_DBG32(TIM_GetCaptureValue(LPC_TIM0,0))
	_DBG("Captured Timer 0 Variable")	
	_DBG32(Timer0_Value)

	/* I am Not Sure of this Configuration.Once you run you will come to know if the timer 
	/* Value is getting updated or not */
	Timer1_Value=TIM_GetCaptureValue(LPC_TIM1,1);
	_DBG("Timer 1 Capture : ");
	_DBG32(TIM_GetCaptureValue(LPC_TIM1,1))
	_DBG("Captured Timer 0 Variable")	
	_DBG32(Timer1_Value)

	/* Subtract Falling Edge Time Value to Rising Edge Time Value 
	   and Multiply by 2 to get 1 Cycle of the Square Wave		
	   The Timer is running in terms of Micro Seconds(i.e 1000000s )
	   So if you Divide the Timer_Interim Value by 100000 then we will get the time in seconds
	   
	   1/(Timer_Interim/10000) ==> 1000000/Timer_Interim will then give the Frequency*/

	Timer_Interim=(Timer1_Value-Timer0_Value) * 2;

	_DBG("Timer Interim Value : ( Timer1- Timer0 ) * 2")
	_DBG32(Timer_Interim)
	_DBG("Frequency Value :")
	/* This will spit the Frequency Value in Hz as Floating Values cannot be directly Stored */
	/* This Line Gives the Actual Frequency Value */
	_DBG32(100000/Timer_Interim)
}

int c_entry(void)
{
	PINSEL_CFG_Type PinCfg,PinCfg1;

	// DeInit NVIC and SCBNVIC
	NVIC_DeInit();
	NVIC_SCBDeInit();

	/* Configure the NVIC Preemption Priority Bits:
	 * two (2) bits of preemption priority, six (6) bits of sub-priority.
	 * Since the Number of Bits used for Priority Levels is five (5), so the
	 * actual bit number of sub-priority is three (3)
	 */
	NVIC_SetPriorityGrouping(0x05);

	//  Set Vector table offset value
#if (__RAM_MODE__==1)
	NVIC_SetVTOR(0x10000000);
#else
	NVIC_SetVTOR(0x00000000);
#endif

	/* Init debug */
	debug_frmwrk_init();

	// print welcome screen
	print_menu();

	/* Config P1.26 as CAP0.0 */
	PinCfg.Portnum = 1;		/* Refers to P1 */
	PinCfg.Pinnum = 26;		/* Refers to P1.26 */
	/* 3 (Function 11) Stands for CAPTURE CAP0.0 */
	PinCfg.Funcnum = 3;		/* Refers to 11(which is 3) for Pin Function(like 00,01,10 and 11) as there are different modes for each pin */
	PinCfg.Pinmode = 0;
	PinCfg.OpenDrain = 0;
	PINSEL_ConfigPin(&PinCfg);

	/* Config P1.27 as CAP0.1 */
	PinCfg.Portnum = 1;		/* Refers to P1 */
	PinCfg.Pinnum = 27;		/* Refers to P1.27 */
	/* 3 (Function 11) Stands for CAPTURE CAP0.1 */
	PinCfg.Funcnum = 3;		/* Refers to 11(which is 3) for Pin Function(like 00,01,10 and 11) as there are different modes for each pin */
	PinCfg1.OpenDrain = 0;
	PinCfg1.Pinmode = 0;
	PINSEL_ConfigPin(&PinCfg1);

	// Initialize timer 0, prescale count time of 1000000uS = 1S
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;

	/* We dont want any prescale Value so Not Giving Any PreScale Count */
/*	TIM_ConfigStruct.PrescaleValue	= 1000000;*/

	/* Enable Capture on Rising Edge for Timer 0 Capture */
	// use channel 0, CAPn.0
	TIM_CaptureConfigStruct.CaptureChannel = 0;
	// Enable capture on CAPn.0 rising edge
	TIM_CaptureConfigStruct.RisingEdge = ENABLE;
	// Disable capture on CAPn.0 falling edge
	TIM_CaptureConfigStruct.FallingEdge = DISABLE;
	// Generate capture interrupt once the Timer Value is Captured
	TIM_CaptureConfigStruct.IntOnCaption = ENABLE;

	/* Enable Capture on Falling Edge for Timer 1 Capture */
	// use channel 1, CAPn.1
	TIM_CaptureConfigStruct1.CaptureChannel = 1;
	// Disable capture on CAPn.0 rising edge
	TIM_CaptureConfigStruct1.RisingEdge = DISABLE;
	// Enable capture on CAPn.0 falling edge
	TIM_CaptureConfigStruct1.FallingEdge = ENABLE;
	// Generate capture interrupt once the Timer Value is Captured
	TIM_CaptureConfigStruct1.IntOnCaption = ENABLE;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);

	TIM_Init(LPC_TIM1, TIM_TIMER_MODE,&TIM_ConfigStruct);
	
	TIM_ConfigCapture(LPC_TIM0, &TIM_CaptureConfigStruct);

	TIM_ConfigCapture(LPC_TIM1, &TIM_CaptureConfigStruct1);

	TIM_ResetCounter(LPC_TIM0);

	TIM_ResetCounter(LPC_TIM1);

	/* At this Time an Interrupt will be Generated by both Timer 0 and Timer 1	*/
	/* Where the Actual Frequency will be calculated		*/

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER0_IRQn);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER1_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 1 */
	NVIC_EnableIRQ(TIMER0_IRQn);

	// To start timer 0
	TIM_Cmd(LPC_TIM0,ENABLE);

	//To start timer 1
	TIM_Cmd(LPC_TIM1,ENABLE);	

	while (1);
	return 1;
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

