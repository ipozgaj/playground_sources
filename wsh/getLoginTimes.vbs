Option Explicit

Dim objFSO, objFile, objWMI, colEvents, objEvent

Const strPath = "vremena.txt"

rem Za Windows 7
Const idSystemStartup  =  12
Const idSystemShutdown =  13

rem Za Windows 7
rem Const idSystemStartup  =  6005
rem Const idSystemShutdown =  6006

Class EventLogDate
	Public Day, Month, Year
	Public Hour, Minute, Second
	
	Sub ParseDate(strDate)		
		Year = Mid(strDate, 1, 4)
		Month = Mid(strDate, 5, 2)
		Day = Mid(strDate, 7, 2)
		Hour = Mid(strDate, 9, 2)
		Minute = Mid(strDate, 11, 2)
		Second = Mid(strDate, 13, 2)
	End Sub
	
	Function GetDate()
		GetDate = Day & "." & Month & "." & Year & " " & Hour & ":" & Minute
	End Function
End Class

Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.CreateTextFile(strPath, True)

Set objWMI = GetObject("winmgmts:{impersonationLevel=impersonate}!\\.\root\cimv2")
Set colEvents = objWMI.ExecQuery("Select * from Win32_NTLogEvent Where Logfile = 'System'" )

For Each objEvent in colEvents
	If objEvent.EventCode = idSystemStartup Then
		Dim startDate
		Set startDate = new EventLogDate
		startDate.ParseDate(objEvent.TimeWritten)
		objFile.WriteLine(startDate.GetDate() & " - Startup")
	End If
	If objEvent.EventCode = idSystemShutdown Then
		Dim endDate
		Set endDate = new EventLogDate
		endDate.ParseDate(objEvent.TimeWritten)
		objFile.WriteLine(endDate.GetDate() & " - Shutdown")
	End if
Next

objFile.Close()