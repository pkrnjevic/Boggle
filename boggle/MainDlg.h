// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StringUtils.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	CSimpleHtmlCtrl m_ctrlHTML;
	CBoard m_board;
	CDice m_dice;

	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_NEW, BN_CLICKED, OnBnClickedNew)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// html/rtf subclass
		m_ctrlHTML.SubclassWindow(GetDlgItem(IDC_BOARD));
		m_ctrlHTML.LimitText(10000);
		WCHAR s[] = L"<TABLE STYLE=\"table-layout:fixed\">" \
			L"<COL WIDTH=30><COL WIDTH=30><COL WIDTH=30><COL WIDTH=30><COL WIDTH=30>" \
			L"<TR>" \
			L"<TD>A</TD><TD>B</TD><TD>C</TD><TD>D</TD><TD>E</TD>" \
			L"</TR>" \
			L"<TR HEIGHT=\"100\">" \
			L"<TD>F</TD><TD>G</TD><TD><strong><FONT COLOR=\"#ff0000\">H</FONT></strong></TD><TD>I</TD><TD>J</TD>" \
			L"</TR>" \
			L"<TD>K</TD><TD>L</TD><TD><strong>M</strong></TD><TD>N</TD><TD>O</TD>" \
			L"</TR>" \
			L"<TD>P</TD><TD>Q</TD><TD>R</TD><TD>S</TD><TD>T</TD>" \
			L"</TR>" \
			L"<TD>U</TD><TD>V</TD><TD>W</TD><TD>X</TD><TD>Y</TD>" \
			L"</TR>" \
			L"</TABLE>";
		m_ctrlHTML.Load(s);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	std::wstring FormatBoard(std::wstring s)
	{
		std::wstring::iterator stor = s.begin();
		std::wostringstream os;
		os << L"<TABLE STYLE=\"table-layout:fixed\">";
		for (int i=0; i<Width; i++)
			os << L"<COL WIDTH=30>";
		for (int j=0; j<Width; j++)
		{
			os << L"<TR>";
			for (int i=0; i<Width; i++)
				os << L"<TD>" << char(*stor++ - 'a'+'A') << L"</TD>";
			os << L"</TR>";
		}
		os << L"</TABLE>";
		return os.str();
	}
#include <ostream>

	LRESULT OnBnClickedNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		std::wstring s = m_dice.RollDice();
		OutputDebugString(s.c_str());
		OutputDebugString(L"\n");
		this->m_ctrlHTML.Load(FormatBoard(s).c_str());
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
};
