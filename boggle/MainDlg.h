// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StringUtils.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CDialogResize<CMainDlg>,
		public CUpdateUI<CMainDlg>,	public CMessageFilter, public CIdleHandler
{
public:
	CSimpleHtmlCtrl m_ctrlHTML;
	CBoard m_board;
	CDice m_dice;
	CDict m_dict;
	CVisitList m_visitlist;

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

	BEGIN_DLGRESIZE_MAP(CMainDlg)
		DLGRESIZE_CONTROL(IDC_RICHEDIT22, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_LIST, DLSZ_MOVE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_NEW, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_SOLVE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(ID_APP_ABOUT, DLSZ_MOVE_X|DLSZ_MOVE_Y)
    END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_NEW, BN_CLICKED, OnBnClickedNew)
		COMMAND_HANDLER(IDC_SOLVE, BN_CLICKED, OnBnClickedSolve)
		COMMAND_HANDLER(IDC_LIST, LBN_SELCHANGE, OnLbnSelchangeList)
        CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		DlgResize_Init(true, true, WS_THICKFRAME|WS_CLIPCHILDREN);

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
		
		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		LoadDict();

		CButton(GetDlgItem(IDC_NEW)).Click();

		return TRUE;
	}

	bool LoadDict()
	{
		// Create a thread and return
		DWORD dwThreadID = 0;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (UINT (WINAPI*)(void*))LoadDictProc, this, 0, (UINT*)&dwThreadID);
		if (hThread == NULL)
			return false;
		::CloseHandle(hThread);	
		AtlMessageBox(m_hWnd,L"Loading word list. Please wait.",L"Working...",0);
		return true;
	}

	static DWORD WINAPI LoadDictProc(void* p)
	{
		CMainDlg* pThis = (CMainDlg*)p;
		pThis->LoadDictHelper();
//		pThis->m_bComplete = true;
		return 0;
	}

	bool LoadDictHelper()
	{		// Load dictionary
		m_dict.Load(L"word.list");
		CWindow wnd(FindWindow(L"#32770",L"Working..."));
		wnd.PostMessageW(WM_CLOSE);
		return true;
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

	LRESULT OnBnClickedNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		std::wstring s = m_dice.RollDice();
		OutputDebugString(s.c_str());
		OutputDebugString(L"\n");
		m_board.Load(s);
		m_ctrlHTML.Load(m_board.Format().c_str());
		BOOL not_used;
		OnBnClickedSolve(0, 0, 0, not_used);
		return 0;
	}
	
	LRESULT OnBnClickedSolve(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_visitlist.clear();
		for (int i=0; i<BoardSize; i++)
			m_board.Walk(i, &m_dict, m_visitlist);
		CListBox lb(GetDlgItem(IDC_LIST));
		while (lb.DeleteString(0) != LB_ERR)
			;
		for (CVisitList::iterator i=m_visitlist.begin(); i!=m_visitlist.end(); i++)
			lb.AddString(m_board.Entries(*i).c_str());
		return 0;
	}
		
	LRESULT OnLbnSelchangeList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		int i = CListBox(hWndCtl).GetCurSel();
		m_ctrlHTML.Load(m_board.Format(-1,&m_visitlist[i]).c_str());
//		OutputDebugString(m_board.Entries(m_visitlist[i]).c_str());
//		OutputDebugString(L"\n");
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

};
