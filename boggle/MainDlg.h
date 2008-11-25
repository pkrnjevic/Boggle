//// MainDlg.h : interface of the CMainDlg class
////
///////////////////////////////////////////////////////////////////////////////
//
//#pragma once
//
//#include "StringUtils.h"
//#include "SimpleHtml.h"
//#include "SplitterBar.h"
//#include "WordListCtrl.h"
//
//class CMainDlg : public CDialogImpl<CMainDlg>, public CDialogResize<CMainDlg>,
//		public CUpdateUI<CMainDlg>,	public CMessageFilter, public CIdleHandler
//{
//public:
//	CDict m_dict;
//	CBoard m_board;
//	CBoardViewCtrl m_boardview;
//	CFont m_font;
//	CDice m_dice;
//	CVertSplitterCtrl m_ctrlSplit;
//	CWordListCtrl m_listview;
//	CWordListCtrl::connection_t m_connection;
//
//	enum { IDD = IDD_MAINDLG };
//
//	CMainDlg() : m_board(&m_dict),
//				 m_boardview(&m_board),
//				 m_listview(&m_board),
//				 m_connection(m_listview.connect(boost::bind(&CMainDlg::OnWordListSelection, this, _1)))
//	{}
//
//	virtual BOOL PreTranslateMessage(MSG* pMsg)
//	{
//		return CWindow::IsDialogMessage(pMsg);
//	}
//
//	virtual BOOL OnIdle()
//	{
//		return FALSE;
//	}
//
//	BEGIN_UPDATE_UI_MAP(CMainDlg)
//	END_UPDATE_UI_MAP()
//
//	BEGIN_DLGRESIZE_MAP(CMainDlg)
//////	DLGRESIZE_CONTROL(IDC_RICHEDIT22, DLSZ_SIZE_X|DLSZ_SIZE_Y)
//		DLGRESIZE_CONTROL(IDC_BOARD, DLSZ_SIZE_X|DLSZ_SIZE_Y)
//		DLGRESIZE_CONTROL(IDC_SPLIT, DLSZ_MOVE_X|DLSZ_SIZE_Y)
//		DLGRESIZE_CONTROL(IDC_LIST, DLSZ_MOVE_X|DLSZ_SIZE_Y)
//		DLGRESIZE_CONTROL(IDC_NEW, DLSZ_MOVE_Y)
////		DLGRESIZE_CONTROL(IDC_SOLVE, DLSZ_MOVE_Y)
//		DLGRESIZE_CONTROL(IDC_SHOW_ME, DLSZ_MOVE_X|DLSZ_MOVE_Y)
//		DLGRESIZE_CONTROL(IDC_SHOW_OTHERS, DLSZ_MOVE_X|DLSZ_MOVE_Y)
//		DLGRESIZE_CONTROL(IDC_SHOW_ALL, DLSZ_MOVE_X|DLSZ_MOVE_Y)
//		DLGRESIZE_CONTROL(ID_APP_ABOUT, DLSZ_MOVE_X|DLSZ_MOVE_Y)
//    END_DLGRESIZE_MAP()
//
//	BEGIN_MSG_MAP(CMainDlg)
//		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
//		MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
////		MSG_WM_MOUSEMOVE(OnMouseMove)
//		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
//		COMMAND_ID_HANDLER(IDOK, OnOK)
//		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
//		COMMAND_ID_HANDLER(IDC_NEW, OnNew)
//		COMMAND_ID_HANDLER(IDC_SOLVE, OnSolve)
//		COMMAND_HANDLER(IDC_LIST, LBN_SELCHANGE, OnLbnSelchangeList)
//// void OnLButtonDown(UINT nFlags, CPoint point)
//		COMMAND_HANDLER(IDC_SHOW_ME, BN_CLICKED, OnBnClickedShow)
//		COMMAND_HANDLER(IDC_SHOW_OTHERS, BN_CLICKED, OnBnClickedShow)
//		COMMAND_HANDLER(IDC_SHOW_ALL, BN_CLICKED, OnBnClickedShow)
//        CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
//	END_MSG_MAP()
//
//// Handler prototypes (uncomment arguments if needed):
////	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
////	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
////	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
//
//	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	{
//		DlgResize_Init(true, true, WS_THICKFRAME|WS_CLIPCHILDREN);
//
////		m_listview.Attach(GetDlgItem(IDC_LIST));
//		m_listview.SubclassWindow(GetDlgItem(IDC_LIST));
//		m_listview.LimitText(100000);
//
//		m_ctrlSplit.SubclassWindow(GetDlgItem(IDC_SPLIT));
//		m_ctrlSplit.SetSplitterPanes(GetDlgItem(IDC_BOARD), GetDlgItem(IDC_LIST));
//
//		// center the dialog on the screen
//		CenterWindow();
//
//		// set icons
//		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
//			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
//		SetIcon(hIcon, TRUE);
//		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
//			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
//		SetIcon(hIconSmall, FALSE);
//
////		// html/rtf subclass
////		m_ctrlHTML.SubclassWindow(GetDlgItem(IDC_BOARD));
////		m_ctrlHTML.LimitText(10000);
//
//		// Set up the owner-draw static control
//		m_boardview.SubclassWindow( GetDlgItem(IDC_BOARD) );
//		
//		// register object for message filtering and idle updates
//		CMessageLoop* pLoop = _Module.GetMessageLoop();
//		ATLASSERT(pLoop != NULL);
//		pLoop->AddMessageFilter(this);
//		pLoop->AddIdleHandler(this);
//
//		UIAddChildWindowContainer(m_hWnd);
//
//		LoadDict();
//
//		//CButton(GetDlgItem(IDC_SHOW_ME)).SetButtonStyle(0x50031003);
//		//CButton(GetDlgItem(IDC_SHOW_OTHERS)).SetButtonStyle(0x50031003);
//		//CButton(GetDlgItem(IDC_SHOW_ALL)).SetButtonStyle(0x50031003);
//
//		CButton(GetDlgItem(IDC_NEW)).Click();
//
//		return TRUE;
//	}
//
//	bool LoadDict()
//	{
//		// Create a thread and return
//		DWORD dwThreadID = 0;
//		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (UINT (WINAPI*)(void*))LoadDictProc, this, 0, (UINT*)&dwThreadID);
//		if (hThread == NULL)
//			return false;
//		::CloseHandle(hThread);	
//		AtlMessageBox(m_hWnd,L"Loading word list. Please wait.",L"Working...",0);
//		return true;
//	}
//
//	static DWORD WINAPI LoadDictProc(void* p)
//	{
//		CMainDlg* pThis = (CMainDlg*)p;
//		pThis->LoadDictHelper();
////		pThis->m_bComplete = true;
//		return 0;
//	}
//
//	bool LoadDictHelper()
//	{		// Load dictionary
//		m_dict.Load(L"word.list");
//		CWindow wnd(FindWindow(L"#32770",L"Working..."));
//		wnd.PostMessageW(WM_CLOSE);
//		return true;
//	}
//
//	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	{
//		// unregister message filtering and idle updates
//		CMessageLoop* pLoop = _Module.GetMessageLoop();
//		ATLASSERT(pLoop != NULL);
//		pLoop->RemoveMessageFilter(this);
//		pLoop->RemoveIdleHandler(this);
//
//		return 0;
//	}
//
//	//void OnMouseMove(UINT nFlags, CPoint point)
//	//{
//	//	m_boardview.MouseLeftClient();
//	//}
//
//	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	{
//		this->m_boardview.Invalidate();
//		return 0;
//	}
//
//	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	{
//		CAboutDlg dlg;
//		dlg.DoModal();
//		return 0;
//	}
//
//	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	{
//		// TODO: Add validation code 
//		CloseDialog(wID);
//		return 0;
//	}
//
//	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	{
//		CloseDialog(wID);
//		return 0;
//	}
//
//	std::wstring FormatBoard(std::wstring s)
//	{
//		std::wstring::iterator stor = s.begin();
//		std::wostringstream os;
//		os << L"<TABLE STYLE=\"table-layout:fixed\">";
//		for (int i=0; i<BoardWidth; i++)
//			os << L"<COL WIDTH=30>";
//		for (int j=0; j<BoardWidth; j++)
//		{
//			os << L"<TR>";
//			for (int i=0; i<BoardWidth; i++)
//				os << L"<TD>" << char(*stor++ - 'a'+'A') << L"</TD>";
//			os << L"</TR>";
//		}
//		os << L"</TABLE>";
//		return os.str();
//	}
//
//	LRESULT OnNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	{
//		std::wstring s = m_dice.RollDice();
//		OutputDebugString(s.c_str());
//		OutputDebugString(L"\n");
//		m_board.Load(s);
//		m_boardview.Update();
//		BOOL not_used;
//		OnSolve(0, 0, 0, not_used);
//		return 0;
//	}
//	
//	LRESULT OnSolve(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	{
//		m_board.Solve();
//		m_listview.Load();
//		return 0;
//	}
//		
//	LRESULT OnLbnSelchangeList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
//	{
//		int i = CListBox(hWndCtl).GetCurSel();
//		m_boardview.Update(i);
////		OutputDebugString(m_board.Entries(m_listview[i]).c_str());
////		OutputDebugString(L"\n");
//		return 0;
//	}
//
//	void OnWordListSelection(const std::wstring& s) 
//	{
//		m_boardview.Update(s);
//	}
//
//	LRESULT OnBnClickedShow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
//	{
//		bHandled = false;
//		CButton b(hWndCtl);
//		UINT state = b.GetState();
//		switch (wID) {
//			case IDC_SHOW_ME:
//				m_listview.ShowMe() = state & BST_CHECKED;
//				break;
//			case IDC_SHOW_OTHERS:
//				m_listview.ShowOthers() = state & BST_CHECKED;
//				break;
//			case IDC_SHOW_ALL:
//				m_listview.ShowAll() = state & BST_CHECKED;
//				break;
//			default:
//				ATLASSERT(false);
//		};
//		m_listview.Update();
//		return 0;
//	}
//
//	void CloseDialog(int nVal)
//	{
//		DestroyWindow();
//		::PostQuitMessage(nVal);
//	}
//
//};
