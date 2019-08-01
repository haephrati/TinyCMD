/*
Tiny CMD - by Michael Haephrati
(c)2019, Michael Haephrati and Secured Globe, Inc.
https://www.securedglobe.net

How to run CMD commands from a c++ based Dialog applicationb
Article first published at CodeProject.
https://www.codeproject.com/Articles/5163375/Tiny-CMD

*/
#include "stdafx.h"
#include "SG_CMD.h"
#define BUFSIZE MAX_PATH
#define RESULTS_FILE L"result.txt"

CString Command, CommandResult;
BOOL IsRunning = FALSE;
void SetCommand(CString command)
{
	Command = command;
	HANDLE hThread = (HANDLE)_beginthread(ThreadFunc, 0, NULL);
}
bool CheckCommandExecutionStatus()
{
	CommandResult = GetResultFromFile();
	if (CommandResult != L"")
		return true;
	else
		return false;
}
void __cdecl ThreadFunc(void*) 
{
	CommandResult = L"";
	if (DoRun(Command.GetBuffer()))
	{
		IsRunning = TRUE;

		while (IsRunning)
		{
			if(CheckCommandExecutionStatus())
			{
				break;
			}
		}
	}
	IsRunning = FALSE;
	_endthreadex(0);
}
CString GetResultFromFile()
{
	CString strResult = L"";
	std::FILE *fp;
	fp = NULL;
	_wfopen_s(&fp, RESULTS_FILE, L"rb");
	if (fp)
	{
		std::string contents;
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
		CString temp1 = (CString)(CStringA)(contents.c_str());
		wprintf(L"Result:\n%s\n", temp1.GetBuffer());
		if (temp1 == L"") temp1 = L"Unknown command";
		strResult = temp1;
	}
	/*else
	{
		DWORD lastError = GetLastError();
		if (lastError == 32)
		{
			// File is locked since it is being prepared
			return strResult;
		}
		strResult.Format(L"Can't open file %s. Error %d", RESULTS_FILE, lastError);
		return strResult;
	}*/
	return strResult;

}

BOOL DoRun(WCHAR *command)
{
	BOOL Result = FALSE;
	DWORD retSize;
	LPTSTR pTemp = NULL;
	TCHAR Command[BUFSIZE] = L"";
	if (!(DeleteFile(RESULTS_FILE)))
	{
		//return L"Can't delete previous results";
	}
	_tcscpy_s(Command, L"/C ");
	_tcscat_s(Command, command);
	_tcscat_s(Command, L" >");
	_tcscat_s(Command, RESULTS_FILE);
	wprintf(L"Calling:\n%s\n", Command);
	Result = (BOOL) ShellExecute(GetActiveWindow(), L"OPEN", L"cmd", Command, NULL, 0L);
	if(!Result)
	{
		retSize = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_ARGUMENT_ARRAY,
			NULL,
			GetLastError(),
			LANG_NEUTRAL,
			(LPTSTR)&pTemp,
			0,
			NULL);
		MessageBox(NULL,pTemp,L"Error",MB_OK);
	}
	return Result;
}
