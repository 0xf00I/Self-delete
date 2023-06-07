#include <windows.h>
#include <tchar.h>

int CommitSuicide(char *szCmdLine)
{
	HANDLE hTemp;
	char szPath[MAX_PATH];
	char szTemp[MAX_PATH];

	static BYTE buf[1024];
	
	STARTUPINFO			si;
	PROCESS_INFORMATION pi;
	UINT ret;

	//open a temporary file
	GetTempPath(MAX_PATH, szTemp);
	lstrcat(szTemp, "suicide.exe");

	GetModuleFileName(0, szPath, MAX_PATH);
	
	CopyFile(szPath, szTemp, FALSE);

	hTemp = CreateFile(szTemp, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, 0,
		OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, 0);

	//Create a process using the temporary executable. This will cause
	//the file's handle count to increase, so we can close it.
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	lstrcat(szTemp, " ");
	lstrcat(szTemp, szCmdLine);

	ret = CreateProcess(0, szTemp, 0, 0, FALSE, NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi);

	//Close our handle to the new process. Because the process is
	//memory-mapped, there will still be a handle held by the O/S, so
	//it won't get deleted. Give the other process a chance to run..
	Sleep(100);
	CloseHandle(hTemp);

	return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR szCmdLine, int iCmdShow)
{
	char szPath[MAX_PATH];

	//
	//	Parse the command line
	//	Normally, we should not be run with any paramters
	//	We re-spawn ourselves with the current module's path
	//
	if(szCmdLine[0] == '\0')
	{
		// Spawn a duplicate process, and tell it to delete US
		HMODULE hModule = GetModuleHandle(0);

		GetModuleFileName(hModule, szPath, MAX_PATH);

		CommitSuicide(szPath);
		
		// Exit normally
		return 0;
	}
	//	This is where we pick up execution second time we run,
	//  When a filename has been specified on the command line
	else
	{
		// Give the calling process time to exit...
		Sleep(200);

		// Delete the file specified on command line.
		DeleteFile(szCmdLine);

		// Exit normally. When we close, this executable will be automatically
		return 0;
	}
}	
