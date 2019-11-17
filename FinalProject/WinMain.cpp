// Program WinMain.cpp

#define STRICT
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "idm.h"
#include "modes.h"
#include "DisplayService.h"
#include "CommController.h"
#include "SessionService.h"
#include "WINDOW.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		WinMain.cpp - A file that contains the program's main method and event processing method.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
--					LRESULT CALLBACK MainProc(HWND, UINT, WPARAM, LPARAM)
--
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:		(N/A)
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- This is the file that the program seeks to run the program.
----------------------------------------------------------------------------------------------------------------------*/

SessionService sessionService;

LRESULT CALLBACK MainProc(HWND, UINT, WPARAM, LPARAM);

#pragma warning (disable: 4096)

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	WinMain
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:
--		DATE:			Oct 11, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Passed displayService pointer to SessionService
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspsqCmdParam, int nCmdShow)
--					HINSTANCE hInst:			instance of the window
--					HINSTANCE hPrevInstance:	previous instance of the window
--					LPSTR lspsqCmdParam:		parameter from the command line
--					int nCmdShow:				visual status of the window (minimized, maximized)
--
-- RETURNS:		int
--
-- NOTES:
-- The program calls this method by default when it starts
----------------------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspsqCmdParam, int nCmdShow)
{
	HWND hwnd;
	MSG Msg;
	WNDCLASSEX Wcl;

	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = 0;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	Wcl.hIconSm = NULL;
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	Wcl.lpfnWndProc = MainProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wcl.lpszClassName = WINDOW_NAME;

	Wcl.lpszMenuName = TEXT("WINMENU");
	Wcl.cbClsExtra = 0;
	Wcl.cbWndExtra = 0;

	if (!RegisterClassEx(&Wcl)) {
		return 0;
	}

	// Configure Window
	hwnd = CreateWindow(
		WINDOW_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		10,
		10,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInst,
		NULL
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	DisplayService displayService = DisplayService{ &hwnd };
	CommController commController = CommController{ &displayService };
	sessionService = SessionService{ &commController, &displayService };

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	MainProc
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	LRESULT CALLBACK MainProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--					HWND hwnd:		window handle
--					UINT Message:	message from the system
--					WPARAM wParam:	low value parameter attached to the message
--					LAPARAM lParam:	high value parameter attached to the message
--
-- RETURNS:		LRESULT
--
-- NOTES:
-- The program handles all windows events
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK MainProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	sessionService.handleProcess(Message, wParam);
	return DefWindowProc(hwnd, Message, wParam, lParam);
}
