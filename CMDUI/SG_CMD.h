/*
Tiny CMD - by Michael Haephrati
(c)2019, Michael Haephrati and Secured Globe, Inc.
https://www.securedglobe.net

How to run CMD commands from a c++ based Dialog applicationb
Article first published at CodeProject.
https://www.codeproject.com/Articles/5163375/Tiny-CMD

*/
#pragma once
void SetCommand(CString command);
bool CheckCommandExecutionStatus();
void __cdecl ThreadFunc(void*);
BOOL DoRun(WCHAR *command);
CString GetResultFromFile();
extern CString Command, CommandResult;
extern BOOL IsRunning;


