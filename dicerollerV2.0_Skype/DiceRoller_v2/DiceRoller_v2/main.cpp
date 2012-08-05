// Skype-DiceRoller.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "SkypeThread.h"

using namespace std;

int main()
{
	HRESULT hr = CoInitialize(NULL);

	DWORD dwThreadId;
	HANDLE hThread;
	SkypeThread obj;

	hThread = CreateThread(NULL,0,SkypeThread::ProcessThread,&obj,0,&dwThreadId);

	//CloseHandle(hThread);
	WaitForSingleObject(hThread, INFINITE);
	
	obj.~SkypeThread();

	
	CoUninitialize();
	return 0;
} 

