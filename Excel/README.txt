Tool	: Just an Illustration to Access/Modify Excel File using Python
Language: Python
OS	: Windows 8/Microsoft Office 2k*
Warning	: Nil
Author  : Vishnu Vasan Nehru
Date    : 09-12-2013


This script is an illustration to access/modify your excel file using win32com client.

The Module xlwt,xlutils and xlrd can only create an object reference by adding a worksheet which makes it inappropriate for working with existing worksheets, so win32com dlls are used as libraries to access the native
methods.This library can be installed from this link http://sourceforge.net/projects/pywin32/files/
Please install the libraries for your respective system configuration and python installation( 32 or 64 bit) 
Or Else the DLLs will not work.
Once installation is complete follow the below steps to check if win32com is installed in your python Directory

1.After the exe installation goto cmd and type python
2.In python prompt type help()
3.In help prompt type modules( This will list you all the modules installed in python)
4.Check if the new win* modules are getting listed especially win32com
5.Once these things exist you are ready to go

Open cmd and type Python Excel.py
It should work with an existing excel file in the mentioned path
