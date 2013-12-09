from win32com.client import Dispatch

# Create an Instance of Excel Application
Excel=Dispatch("Excel.Application")

#Open your Excel WorkBook
Book=Excel.Workbooks.Open("F:\\Python\\Excel\\simple.xls")

#Select your Work Sheet
Sheet=Book.Worksheets(1)

# Select the Active Workbook
Current_Workbook=Excel.ActiveWorkbook

#Write your values
Sheet.Cells(2,3).Value = "Prakash"
Sheet.Cells(4,5).Value = "Karisidappa"
Sheet.Cells(10,1).Value = "Robert BOSCH"

#Save the Workbook
Current_Workbook.Save()

#Once Saved Close the Workbook
Current_Workbook.Close()

#Quit the Excel Application
Excel.Quit()

################################################################################################
#
# Additional Information are available in these links
#
# Win32 Modules for Python
# http://python.net/crew/mhammond/win32/Downloads.html
# http://sourceforge.net/projects/pywin32/files/
#
# Different Methods in Excel Files using Win32COM
# https://mail.python.org/pipermail/python-win32/2011-August/011738.html
# http://docs.activestate.com/activepython/2.4/pywin32/html/com/win32com/HTML/docindex.html
#
###############################################################################################
