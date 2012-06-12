' MQ klijent za slanje i primanje datoteka
' Zadnja izmjena: 17.12.2008
' Argumenti: [send|receive] [QueueManager] [Queue] [File]

Option Explicit

Const MQOO_INPUT_AS_Q_DEF   = 1
Const MQOO_OUTPUT           = 16
Const adTypeBinary          = 1
Const adSaveCreateOverwrite = 2
	
Dim data
Dim objArgs, argMode, argQueueManager, argQueue, argFileName
Dim objMQSession, objMQQueueManager, objMQQueue, objMQMessage, objMQMessageOptions

Set objArgs = WScript.Arguments

If objArgs.Length <> 4 Then
	WScript.Echo "Usage: MQFileClient.vbs [send|receive] [QueueManager] [Queue] [File]"
	WScript.Quit
End If

argMode         = objArgs(0)
argQueueManager = objArgs(1)
argQueue        = objArgs(2)
argFileName     = objArgs(3)

Set objMQSession = CreateObject("MQAX200.MQSession")
Set objMQQueueManager = objMQSession.AccessQueueManager(argQueueManager)

If argMode = "send" Then
	Set objMQQueue = objMQQueueManager.AccessQueue(argQueue, MQOO_OUTPUT)
	Set objMQMessage = objMQSession.AccessMessage()
	
	objMQMessage.ClearMessage
	objMQMessage.Format = "MQSTR"

	data = ReadBinaryFile(argFileName)
	objMQMessage.Write(data)
	objMQQueue.Put(objMQMessage)
Else
	Set objMQQueue = objMQQueueManager.AccessQueue(argQueue, MQOO_INPUT_AS_Q_DEF)
	Set objMQMessage = objMQSession.AccessMessage()
	Set objMQMessageOptions = objMQSession.AccessGetMessageOptions()

	objMQQueue.Get objMQMessage, objMQMessageOptions
	
	data = objMQMessage.Read(objMQMessage.MessageLength)
	WriteBinaryFile argFileName, data
End If

objMQQueue.Close
objMQQueueManager.Disconnect
Set objMQSession = Nothing

' Binary file operations
Function ReadBinaryFile(filename)
	Dim objBinaryStream
	Set objBinaryStream = CreateObject("ADODB.Stream")
	
	objBinaryStream.Type = adTypeBinary
	objBinaryStream.Open	
	objBinaryStream.LoadFromFile(filename)
	
	ReadBinaryFile = objBinaryStream.Read
End Function

Sub WriteBinaryFile(filename, buffer)
	Dim objBinaryStream
	Set objBinaryStream = CreateObject("ADODB.Stream")
	
	objBinaryStream.Type = adTypeBinary
	objBinaryStream.Open
	objBinaryStream.Write buffer
	objBinaryStream.SaveToFile filename, adSaveCreateOverwrite
End Sub