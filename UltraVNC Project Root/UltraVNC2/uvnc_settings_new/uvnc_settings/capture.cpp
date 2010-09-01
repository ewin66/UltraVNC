//  Copyright (C) 2010 Ultr@VNC Team Members. All Rights Reserved.
// 
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
// If the source code for the program is not available from the place from
// which you received this file, check 
// http://www.uvnc.com/
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
/*IDC_POLL_FULLSCREEN
IDC_POLL_FOREGROUND
IDC_POLL_UNDER_CURSOR
IDC_CONSOLE_ONLY
IDC_POLL_UNDER_CURSOR
IDC_ONEVENT_ONLY
IDC_TURBOMODE
IDC_HOOK
IDC_DRIVER
IDC_ALPHA
IDC_REMOVE_Aero
IDC_REMOVE_WALLPAPER
IDC_CHECKDRIVER*/

extern LONG Primary;
extern LONG Secundary;
extern LONG TurboMode;
extern LONG PollUnderCursor;
extern LONG PollForeground;
extern LONG PollFullScreen;
extern LONG PollConsoleOnly;
extern LONG PollOnEventOnly;
extern LONG Driver;
extern LONG Hook;
extern LONG CaptureAlphaBlending;
extern LONG RemoveWallpaper;
extern LONG RemoveAero;
bool CheckVideoDriver(bool Box);

bool initdone5=false;
BOOL CALLBACK DlgProcCAP(HWND hwnd, UINT uMsg,
											   WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg) {
		
	case WM_INITDIALOG: 
		{	
			initdone5=false;
			SendMessage(GetDlgItem(hwnd, IDC_TURBOMODE), BM_SETCHECK, TurboMode, 0);
			SendMessage(GetDlgItem(hwnd, IDC_DRIVER),BM_SETCHECK,Driver,0);
			SendMessage(GetDlgItem(hwnd, IDC_HOOK),BM_SETCHECK,Hook,0);
			SendMessage(GetDlgItem(hwnd, IDC_POLL_FULLSCREEN),BM_SETCHECK,PollFullScreen,0);
			SendMessage(GetDlgItem(hwnd, IDC_POLL_FOREGROUND),BM_SETCHECK,PollForeground,0);
			SendMessage(GetDlgItem(hwnd, IDC_POLL_UNDER_CURSOR),BM_SETCHECK,PollUnderCursor,0);
			SendMessage(GetDlgItem(hwnd, IDC_CONSOLE_ONLY),BM_SETCHECK,PollConsoleOnly,0);
			SendMessage(GetDlgItem(hwnd, IDC_ALPHA), BM_SETCHECK,CaptureAlphaBlending, 0);
			SendMessage(GetDlgItem(hwnd, IDC_REMOVE_WALLPAPER),BM_SETCHECK,RemoveWallpaper,0);
			SendMessage(GetDlgItem(hwnd, IDC_REMOVE_Aero),BM_SETCHECK,RemoveAero,0);
			SendMessage(GetDlgItem(hwnd, IDC_ONEVENT_ONLY),BM_SETCHECK,PollOnEventOnly,0);
			
			EnableWindow(GetDlgItem(hwnd, IDC_CONSOLE_ONLY),PollUnderCursor ||PollForeground);
			EnableWindow(GetDlgItem(hwnd, IDC_ONEVENT_ONLY),PollUnderCursor ||PollForeground);

			SendMessage(GetDlgItem(hwnd, IDC_PRIM),BM_SETCHECK,Primary,0);
			SendMessage(GetDlgItem(hwnd, IDC_SEC),BM_SETCHECK,Secundary,0);

			initdone5=true;
			return TRUE;
		}
	
	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{	
		case IDOK:	
			{
				TurboMode=SendDlgItemMessage(hwnd, IDC_TURBOMODE, BM_GETCHECK, 0, 0);
				PollUnderCursor=SendDlgItemMessage(hwnd, IDC_POLL_UNDER_CURSOR, BM_GETCHECK, 0, 0);
				PollForeground=SendDlgItemMessage(hwnd, IDC_POLL_FOREGROUND, BM_GETCHECK, 0, 0);
				PollFullScreen=SendDlgItemMessage(hwnd, IDC_POLL_FULLSCREEN, BM_GETCHECK, 0, 0);
				PollConsoleOnly=SendDlgItemMessage(hwnd, IDC_CONSOLE_ONLY, BM_GETCHECK, 0, 0);
				PollOnEventOnly=SendDlgItemMessage(hwnd, IDC_ONEVENT_ONLY, BM_GETCHECK, 0, 0);
				Driver=SendDlgItemMessage(hwnd, IDC_DRIVER, BM_GETCHECK, 0, 0);
				Primary=SendDlgItemMessage(hwnd, IDC_PRIM, BM_GETCHECK, 0, 0);
				Secundary=SendDlgItemMessage(hwnd, IDC_SEC, BM_GETCHECK, 0, 0);
				Hook=SendDlgItemMessage(hwnd, IDC_HOOK, BM_GETCHECK, 0, 0);
				CaptureAlphaBlending=SendDlgItemMessage(hwnd, IDC_ALPHA, BM_GETCHECK, 0, 0);
				RemoveWallpaper=SendDlgItemMessage(hwnd, IDC_REMOVE_WALLPAPER, BM_GETCHECK, 0, 0);
				RemoveAero=SendDlgItemMessage(hwnd, IDC_REMOVE_Aero, BM_GETCHECK, 0, 0);
			}
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			return TRUE;
		case IDC_POLL_FOREGROUND:
		case IDC_POLL_UNDER_CURSOR:
			// User has clicked on one of the polling mode buttons
			// affected by the pollconsole and pollonevent options
			{
				// Get the poll-mode buttons
				HWND hPollForeground = GetDlgItem(hwnd, IDC_POLL_FOREGROUND);
				HWND hPollUnderCursor = GetDlgItem(hwnd, IDC_POLL_UNDER_CURSOR);

				// Determine whether to enable the modifier options
				BOOL enabled = (SendMessage(hPollForeground, BM_GETCHECK, 0, 0) == BST_CHECKED) ||
					(SendMessage(hPollUnderCursor, BM_GETCHECK, 0, 0) == BST_CHECKED);

				HWND hPollConsoleOnly = GetDlgItem(hwnd, IDC_CONSOLE_ONLY);
				EnableWindow(hPollConsoleOnly, enabled);

				HWND hPollOnEventOnly = GetDlgItem(hwnd, IDC_ONEVENT_ONLY);
				EnableWindow(hPollOnEventOnly, enabled);
			}
			return TRUE;
		case IDC_CHECKDRIVER:
			{
				CheckVideoDriver(1);
			}
			return TRUE;
	}
		return 0;	
	}

	return 0;
}