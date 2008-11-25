// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler, public CToolBarHelper<CMainFrame>
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CBoggleView m_view;

	CCommandBarCtrl m_CmdBar;
	CToolBarCtrl m_tbar;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
//		COMMAND_ID_HANDLER(ID_FILE_NEW_WINDOW, OnFileNewWindow)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP(CToolBarHelper<CMainFrame>)
		if (uMsg == WM_COMMAND)
			CHAIN_MSG_MAP_MEMBER(m_view)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE_EX);
		m_tbar = hWndToolBar;

//		AddToolbarButtonText(hWndToolBar, ID_FILE_NEW, _T("Cool"));
		AddToolbarButtonText(hWndToolBar, IDC_NEW, _T("New"));
		AddToolbarButtonText(hWndToolBar, IDC_SHOW_ME, _T("Me"));
		AddToolbarButtonText(hWndToolBar, IDC_SHOW_ALL, _T("All"));
		AddToolbarButtonText(hWndToolBar, ID_APP_ABOUT, _T("About"));

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		// Comment next line to remove menu
//		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();

		m_hWndClient = m_view.Create(m_hWnd);

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//m_tbar.ModifyStyle(0,TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_ALTDRAG | TBSTYLE_EX_MIXEDBUTTONS);
		//TCHAR s[100] = _T("hi there");
		//TCHAR s1[100] = {0};
		//TBBUTTONINFO tbbi = {sizeof(tbbi),TBIF_STATE|TBIF_STYLE|TBIF_TEXT,0};
		//tbbi.pszText = s1;
		//tbbi.cchText = sizeof(s1);

		//m_tbar.GetButtonInfo(ID_FILE_NEW,&tbbi);
		//tbbi.dwMask |= TBIF_TEXT;
		//tbbi.pszText = s;
		//tbbi.cchText = sizeof(s);
		//tbbi.fsStyle |= BTNS_SHOWTEXT;
		//m_tbar.SetButtonInfo(ID_FILE_NEW,&tbbi);		// TODO: add code to initialize document

		m_tbar.ModifyStyle(TBSTYLE_LIST,0);

		return 0;
	}

	LRESULT OnFileNewWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		::PostThreadMessage(_Module.m_dwMainThreadID, WM_USER, 0, 0L);
		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
};
