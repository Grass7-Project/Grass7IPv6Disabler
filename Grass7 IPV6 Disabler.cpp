// IPv6 Disabler for Grass7, no longer used due to changes.

#include "stdafx.h"
#include <windows.h>
#include <shlobj.h>
#include <NTSecAPI.h>
#include <string>
#include <Gr7API.h>

int main(void)
{
	if(gr7::isProcessElevated(GetCurrentProcess()) != 1) {
		TaskDialog(NULL, NULL, gr7::LoadOSBrandingString(), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UAC_ERROR_HEADER), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UAC_ERROR_TEXT), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

	HRESULT hr;
	TASKDIALOGCONFIG tdc = { sizeof(TASKDIALOGCONFIG) };
	int nClickedBtn;
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN_ICON));
	LPCWSTR szTitle = gr7::LoadOSBrandingString(),
	  szHeader = gr7::LoadStringToW(GetModuleHandle(NULL), IDS_HEADER),
	  szBodyText = gr7::LoadStringToW(GetModuleHandle(NULL), IDS_BODY_TEXT);
	TASKDIALOG_BUTTON aCustomButtons[] = {
	  { 1000, gr7::LoadStringToW(GetModuleHandle(NULL),IDS_BUTTON_ENABLE) },
	  { 1001, gr7::LoadStringToW(GetModuleHandle(NULL),IDS_BUTTON_DISABLE) },
	  { 1002, gr7::LoadStringToW(GetModuleHandle(NULL),IDS_BUTTON_EXIT) }
	};
 
	  tdc.hwndParent = NULL;
	  tdc.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION|TDF_USE_COMMAND_LINKS|TDF_USE_HICON_MAIN;
	  tdc.pButtons = aCustomButtons;
	  tdc.cButtons = _countof(aCustomButtons);
	  tdc.pszWindowTitle = szTitle;
	  tdc.hMainIcon = hIcon;
	  tdc.pszMainInstruction = szHeader;
	  tdc.pszContent = szBodyText;
 
	  hr = TaskDialogIndirect ( &tdc, &nClickedBtn, NULL, NULL );
 
	  if ( SUCCEEDED(hr) && 1000 == nClickedBtn )
		{
			HKEY keyHandle1;
			RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\services\\TCPIP6\\Parameters"), NULL, KEY_ALL_ACCESS, &keyHandle1);
			DWORD value=0;
			RegSetValueEx(keyHandle1, TEXT("DisabledComponents"), 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
			RegCloseKey(keyHandle1);
			RestartDialogEx(NULL, _T("TEST"), EWX_REBOOT, NULL);
		}
	  if ( SUCCEEDED(hr) && 1001 == nClickedBtn )
		{
			HKEY keyHandle1;
			RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\services\\TCPIP6\\Parameters"), NULL, KEY_ALL_ACCESS, &keyHandle1);
			DWORD value=255;
			RegSetValueEx(keyHandle1, TEXT("DisabledComponents"), 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
			RegCloseKey(keyHandle1);
			RestartDialogEx(NULL, _T("TEST"), EWX_REBOOT, NULL);
		}
	return 0;
}

