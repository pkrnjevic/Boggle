#pragma once

//#ifndef _SIMPLEHTMLCTRL_H_
//#error WordListCtrl.h requires SimpleHtml.h to be included first
//#endif

#ifndef _MAP_
#error WordListCtrl.h requires map to be included first
#endif

#ifndef BOOST_SIGNAL_HPP
  #error WordListCtrl.h requires boost/signal to be included first
#endif

#ifndef __BOARD_H__
  #error WordListCtrl.h requires board.h to be included first
#endif

#include "TOM.h"


// NB: CWordListCtrl wraps a control to display dict.h's CWordList. However, the implemention
// is very different. CWordList is a deque of int lists while this is a SimpleHtmlCtrl displaying 
// std::wstrings.
//
template<class T>
class CWordListBase
{
	//	typedef CRichEditCtrl baseClass;
	typedef std::list<std::wstring> string_list;
	typedef std::map<int,string_list> strings_map;
	strings_map m_list;
	const CBoard& m_board;
	CBoard::connection_t m_connection;
	bool m_show_me;
	bool m_show_others;
	bool m_show_all;

public:
	static const int all = -1;
	static const int me = 0;
	static const int others = 1;

	CWordListBase(CBoard* pb) : m_board(*pb),
		m_connection(m_board.connect(boost::bind(&CWordListCtrl::refresh, this, _1))),
		m_show_me(true), 
		m_show_others(false), 
		m_show_all(false)
	{
		// CBoard& bd = *(const_cast<CBoard*>(&m_board));
		// const CBoard& bd = m_board;
	}

	virtual ~CWordListBase(void)
	{
	}

	bool& ShowMe() { return m_show_me; }
	bool& ShowOthers() { return m_show_others; }
	bool& ShowAll() { return m_show_all; }
	void Append(const std::wstring& s, int user) 
	{ 
		m_list[user].push_back(s); 
	}

	void Load()
	{
		m_list.clear();
		const CWordList& wordlist = m_board.GetWordList();
		std::wstring prev_word = L"";
		for (CWordList::const_iterator i=wordlist.begin(); i!=wordlist.end(); i++)
		{
			CWord word = *i;
			if (word == prev_word)
				continue;
			prev_word = word;
			std::wstring s = (i == wordlist.begin() ? L"" : L", ") + word;
			this->Append(s,-1);
		}
		this->Update();
	}

	void Update() 
	{
		static_cast<T*>(this)->SetWindowText(_T(""));

		for (strings_map::const_iterator& j=m_list.begin(); j!=m_list.end(); j++)
		{
			int user = j->first;
			const string_list& sl = j->second;
			if (user == all && !m_show_all)
				continue;
			else if (user == me && !m_show_me)
				continue;
			else if (user == all && !m_show_all)
				continue;
			for (string_list::const_iterator i = sl.begin(); i!=sl.end(); i++)
			{
				static_cast<T*>(this)->AppendText(i->c_str());
			}
		}
	}
private:
	void refresh(CBoard::state s)
	{
		if (s == CBoard::state::new_word)
		{
			m_list[0].clear();
			const CWordList& wordlist = m_board.GetUserWordList();
			for (CWordList::const_iterator i=wordlist.begin(); i!=wordlist.end(); i++)
			{
				std::wstring word = *i;
				//				if (word == prev_word)
				//					continue;
				//				prev_word = word;
				std::wstring s = (i == wordlist.begin() ? L"" : L", ") + word;
				this->Append(s,0);
			}
			this->Update();
		}
	}
};



/////////////////////////////////////////////////////////////////////////////
// CSimpleHtmlCtrl - a subclassed RichEdit control

template< class T, class TBase = CRichEditCtrl, class TWinTraits = CControlWinTraits >
class CWordListImpl : public CWordListBase<T>, public CWindowImpl< T, TBase, TWinTraits >, public CRichEditCommands<T>
{
public:
	enum state {mouse};
	typedef boost::signal<void (const std::wstring&)>  signal_t;
    typedef boost::signals::connection  connection_t;
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())
private:
	CComQIPtr<ITextDocument> m_tom;
	signal_t m_sig;
	bool m_in_client;

public:
	CWordListImpl(CBoard* pb) : CWordListBase<T>(pb), 
								m_in_client(false)
	{
	}

	// NB: not entirely const: the m_sig is modified as subscribers are added
    connection_t connect(typename signal_t::slot_function_type subscriber) 
    {
        return reinterpret_cast<T*>(this)->m_sig.connect(subscriber);
    }

    void disconnect(connection_t subscriber)
    {
        subscriber.disconnect();
    }

	// Operations

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
		TCHAR szBuffer[20];
		if( ::GetClassName(hWnd, szBuffer, (sizeof(szBuffer)/sizeof(TCHAR))-1) ) {
			ATLASSERT(::lstrcmpi(szBuffer, TBase::GetWndClassName()) == 0);
		}
#endif
		BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
		if(bRet) _Init();
		return bRet;
	}


	// Implementation

	void _Init()
	{
		// Forgot to load RTF library?
		ATLASSERT(::GetModuleHandle(CRichEditCtrl::GetLibraryName())!=NULL);
		// Some style that should be set...
		ATLASSERT((GetStyle() & (ES_MULTILINE|ES_AUTOVSCROLL))==(ES_MULTILINE|ES_AUTOVSCROLL));
		// Default background color
		//      m_clrBack = ::GetSysColor(COLOR_WINDOW);
		// Turn on word-wrapping...
		ModifyStyle(ES_AUTOHSCROLL, 0);
		SetTargetDevice(NULL, 0);
		// We want to have nice URL behaviour
		SetEventMask(ENM_SELCHANGE | ENM_LINK);

		IRichEditOle*pOle =  this->GetOleInterface();
		if(pOle)
		{
			m_tom = pOle;
			pOle->Release();
		}
	}

	// Message map and handlers

	BEGIN_MSG_MAP(baseClass)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
//		MSG_WM_NCHITTEST(OnNcHitTest)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		CHAIN_MSG_MAP_ALT(CRichEditCommands<T>, 1)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		// First let original control initialize everything
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		_Init();
		return lRet;
	}

	UINT OnNcHitTest(CPoint point)
	{
		return HTCLIENT;
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		SetMsgHandled(false);
		if (!m_in_client)
		{
			m_in_client = true;
			StartTrackMouseLeave();
		}
		this->ClientToScreen(&point);
		CHARRANGE range = {0,0};
//		m_bSpellEnabled = m_colorizer.GetSelFromPos(point.x,point.y,range);
//		OutputDebugString(s);

		CComPtr<ITextRange> pRange;
		if (m_tom->RangeFromPoint(point.x,point.y,&pRange) !=  S_OK)
			return;

		pRange->Expand(tomWord,NULL);
		// remove spaces 
		CComVariant var(C1_SPACE|C1_PUNCT);
		pRange->MoveEndWhile(&var,tomBackward,NULL);

		long start;
		long end;
		pRange->GetStart(&start);
		pRange->GetEnd(&end);
		pRange->Select();

		if (end <= start) 
			return;

		CComBSTR str;
		pRange->GetText(&str);
//		CStringA s(str);
		std::wstring s(str);

		AtlTrace("mouse x:%d y:%d min:%d max:%d word:%s\n",point.x,point.y,start,end,s.c_str());
		m_sig(s);
	}

	void OnMouseLeave()
	{
		m_in_client = false;
		std::wstring s;
		m_sig(s);
	}

	BOOL StartTrackMouseLeave()
	{
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		return _TrackMouseEvent(&tme);
	}

};


class CWordListCtrl : public CWordListImpl<CWordListCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_WordListCtrl"), GetWndClassName())

	CWordListCtrl(CBoard* pb) : CWordListImpl<CWordListCtrl>(pb)
	{
	}

};
