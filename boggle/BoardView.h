#pragma once

#define __BOARDVIEW_H__

#include <math.h>


inline double Radians(double degrees)
{
	const double pi = 3.14159265358979323846;
	return degrees / (180. / pi);
}

template <class T, int _size> 
struct array_wrapper
{
	//typedef names used in STL algorithms and containers
	typedef T value_type;
	typedef T* iterator;
	typedef const T * const_iterator;
	typedef T& reference;
	typedef const T& const_reference;

	T v[_size]; //the actual array
	// member functions of STL containers
	operator T* () {return v;}
	reference operator[] (size_t idx) {return v[idx];}
	const_reference operator[] (size_t idx) const {return v[idx];}

	iterator begin() {return v;}
	const_iterator begin() const {return v;}

	iterator end() {return v+_size;}
	const_iterator end() const {return v+_size;}

	size_t size() const {return _size;}
};

class CTriangle : public array_wrapper<CPoint,7>
{
public:
	CTriangle()
	{
		CPoint* ppt = v;
		ppt[0] = CPoint(-3,-3);
		ppt[1] = CPoint(0,-3);
		ppt[2] = CPoint(0,-7);
		ppt[3] = CPoint(4,-7);
		ppt[4] = CPoint(4,-3);
		ppt[5] = CPoint(7,-3);
		ppt[6] = CPoint(2,7);
	}
	void Rotate(int angle)
	{
		double radians = Radians(angle);
		for (int i=0; i<size(); i++)
			CTriangle::Rotate(v[i],radians);
	}
private:
	static void Rotate(CPoint& pt,double radians)
	{
		CPoint o;
		o.x = pt.x*cos(radians)-pt.y*sin(radians)+.5;
		o.y = pt.x*sin(radians)+pt.y*cos(radians)+.5;
		pt = o;
	}
};

class CTriangleList
{
	typedef std::vector<CTriangle> listType;
	listType m_list;
public:
	CTriangleList()
	{
		static int angles[8] = {135, 180, -135, 90, -90, 45, 0, -45};
		m_list.resize(8);
		for (int i=0; i<m_list.size(); i++)
			m_list[i].Rotate(angles[i]); 
	}
	CTriangle& operator[](int n)
	{
		return m_list[n];
	}
};

template< class T, class TBase = CStatic, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CBoardViewImpl : public CWindowImpl< T, TBase, TWinTraits >
{
	typedef class CWindowImpl<T,TBase,TWinTraits> baseClass;
public:
	CFont m_font;
	CBoard& m_board;
	int m_selection;			// item selected in listbox
	CTriangleList m_triangles;
	CRect m_rect;				// client rectangle
	int m_mouse_square;			// square mouse is over or -1 if none
	bool m_in_client;
	CWord m_word;
	bool m_is_word;
	ULONGLONG m_mouse_time;		// mouse time (system ticks) in msec

	CBoardViewImpl(CBoard* pb) : m_board(*pb), 
								 m_font(0),
								 m_selection(-1),
								 m_in_client(false),
								 m_is_word(false),
								 m_mouse_time(0)
	{}

	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
		TCHAR szBuffer[20];
		if( ::GetClassName(hWnd, szBuffer, (sizeof(szBuffer)/sizeof(TCHAR))-1) )
			ATLASSERT(::lstrcmpi(szBuffer, TBase::GetWndClassName()) == 0);
#endif
		BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
		if(bRet) Init();
		return bRet;
	}

	void Init()
	{
		ModifyStyle ( SS_TYPEMASK, SS_OWNERDRAW );
		CSize size(0,0);
		this->OnSize(0,size);
	}

	void Update(int item = -1) 
	{
		m_board.SetSelection(item);
		SetWindowTextW(m_board.Format().c_str());
	}

	void Update(const std::wstring& s) 
	{
		const CWord& word(m_board.String2Word(s));
		int i = m_board.Find(word);
		ATLASSERT(i != -1);
		this->Update(i);
	}

	// Message map and handlers

	BEGIN_MSG_MAP(CBoardViewImpl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SIZE(OnSize)
		MSG_WM_NCHITTEST(OnNcHitTest)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONDOWN(OnMouseDown)
		MSG_WM_LBUTTONUP(OnMouseUp)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
//        MSG_WM_DRAWITEM(OnDrawItem)
//        MSG_WM_PAINT(OnPaint)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		// First let original control initialize everything
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		Init();
		return lRet;
	}

	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		SetMsgHandled(true);
		return TRUE;
	}

	void OnSize(UINT nType, CSize size)
	{
		if (GetCurrentMessage())
			SetMsgHandled(false);
		ATLASSERT(m_hWnd);

		// Determine what font to use for the text.
		LOGFONT lf = {0};
		//			if ( !IsThemeNull() )
		//				GetThemeSysFont ( TMT_MSGBOXFONT, &lf );
		//			else
		{
			NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };

			SystemParametersInfo ( SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, false );
			lf = ncm.lfMessageFont;
		} 

		//			lf.lfHeight = -nPos;
		if (m_font)
			m_font.DeleteObject();
		CRect rc;
		GetClientRect(rc);
		m_rect = rc;
		lf.lfHeight = rc.Height() / 7;
//		lf.lfWidth = rc.Width() / 7;
		m_font.CreateFontIndirect ( &lf ); 		
		//			this->Invalidate(false);
		Invalidate();
	}

	UINT OnNcHitTest(CPoint point)
	{
		return HTCLIENT;
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		if (!m_in_client)
		{
			m_in_client = true;
			StartTrackMouseLeave();
		}
		int row = point.y * BoardWidth / m_rect.Height();
		int col = point.x * BoardWidth / m_rect.Width();
		AtlTrace("flags=%d,x/y=[%d,%d],row/col=[%d,%d]\n",nFlags,point.x,point.y,row,col);
		int square = col + row * BoardWidth;
		if (square != m_mouse_square && nFlags & MK_LBUTTON)
		{
			UINT hover_time;
			SystemParametersInfo(SPI_GETMOUSEHOVERTIME,0,&hover_time,0);
			ULONGLONG time = GetTickCount64();
			if (time - m_mouse_time < hover_time)
			{
				m_word.pop_back();
			}
			m_mouse_time = time;
			m_mouse_square = square;
			m_word.push_back(square);
			const std::wstring& s = m_board.Word2String(m_word);
			m_is_word = m_board.Lookup(s);
			std::wstring ss;
			ss += L"debug: ";
			ss += s;
			ss += m_is_word ? L" true" : L" false";
			ss += L"\n";
			OutputDebugString(ss.c_str());
			this->Invalidate();
		}
	}

	void OnMouseDown(UINT nFlags, CPoint point)
	{
		m_word.clear();
		m_mouse_square = -1;
		return OnMouseMove(nFlags,point);
	}

	void OnMouseUp(UINT nFlags, CPoint point)
	{
		m_board.AddUserWord(m_word);
		m_word.clear();
		this->Invalidate();
	}

	void OnMouseLeave()
	{
		m_in_client = false;
		if (m_mouse_square != -1)
		{
			m_mouse_square = -1;
			this->Invalidate();
		}
	}

	//void OnSize(UINT nType, CSize size)
	//{
	//	SetMsgHandled(false);
	//	m_boardview.OnSize(nType, size);
	//}

//	void OnDrawItem ( UINT uID, LPDRAWITEMSTRUCT lpdis )
//	void OnPaint(CDCHandle dc)
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		T* pT = static_cast<T*>(this);
		//if( wParam != NULL )
		//{
		//	CMemoryDC memdc((HDC)wParam, NULL);
		//	pT->DoPaint(memdc.m_hDC);
		//}
		//else
		{
			RECT rc;
			::GetClientRect(pT->m_hWnd, &rc);
			CPaintDC dc(pT->m_hWnd);
			CMemoryDC memdc(dc.m_hDC, rc);
			pT->DoPaint(memdc.m_hDC,rc);
		}
		return 0;
	}

	void DoPaint(CDCHandle dc, const CRect& rcCtrl)
	{
		CString sText;

		this->GetWindowTextW( sText );

		//if ( IsCompositionEnabled() )
		//{
		//	// Set up a memory DC where we'll draw the text.
		//	CDC dcMem;

		//	dcMem.CreateCompatibleDC ( dc );
		//	dcMem.SaveDC();

		//	// Create a 32-bit bmp for use in offscreen drawing when glass is on
		//	BITMAPINFO dib = {0};
		//	CBitmap bmp;

		//	dib.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		//	dib.bmiHeader.biWidth = rcCtrl.Width();
		//	dib.bmiHeader.biHeight = -rcCtrl.Height();  // negative because DrawThemeTextEx() uses a top-down DIB
		//	dib.bmiHeader.biPlanes = 1;
		//	dib.bmiHeader.biBitCount = 32;
		//	dib.bmiHeader.biCompression = BI_RGB;

		//	bmp.CreateDIBSection ( dc, &dib, DIB_RGB_COLORS, NULL, NULL, 0 );

		//	// Set up the DC state.
		//	DTTOPTS dto = { sizeof(DTTOPTS) };

		//	dto.dwFlags = DTT_COMPOSITED|DTT_GLOWSIZE;
		//	dto.iGlowSize = 10;

		//	dcMem.SelectBitmap ( bmp );
		//	dcMem.SelectFont ( m_font );

		//	DrawThemeTextEx ( dcMem, 0, 0, CT2CW(sText), -1, uFormat, rcCtrl, &dto );

		//	dc.BitBlt ( rcCtrl.left, rcCtrl.top, rcCtrl.Width(), rcCtrl.Height(), dcMem, 0, 0, SRCCOPY );

		//	dcMem.RestoreDC(-1);
		//}
		//else
		//{
			ATLASSERT(BoardSize == sText.GetLength());
			dc.SaveDC();

			dc.FillSolidRect ( rcCtrl, GetSysColor(COLOR_3DFACE) );

			dc.SetBkMode ( TRANSPARENT );
			dc.SetTextColor ( GetSysColor(COLOR_WINDOWTEXT) );
			dc.SelectFont ( m_font );
			for (int square=0; square < BoardSize; square++) 
			{
				CRect rc = GetRect(square);
				DrawChar(dc, sText[square], square);
			}
			for (int square=0; square < BoardSize; square++) 
				DrawTriangle(dc,square);
			dc.RestoreDC(-1);
//		}
	}

	void DrawChar(CDCHandle dc, char ch, int square)
	{
		const CRect rc = GetRect(square);
		const UINT uFormat = DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX;
		CWord::iterator i = std::find(m_word.begin(), m_word.end(), square);
		bool mouse_over = m_word.end() != i;
		bool in_word = m_board.IsSquareUsed(square);
		UINT button_style = DFCS_BUTTONPUSH | (in_word | mouse_over ? DFCS_CHECKED : 0);
		dc.DrawFrameControl(const_cast<CRect&>(rc), DFC_BUTTON, button_style);
		//dc.FrameRect(rc,(HBRUSH)::GetStockObject(BLACK_BRUSH));
		const wchar_t wch = ch;
		COLORREF red(0x000000ff);
		COLORREF green(0x0000ff00);
		COLORREF blue(0x00ff0000);
		COLORREF black(0x00000000);
		COLORREF cr = in_word ? blue : black;
		cr = mouse_over ? (m_is_word ? green : red) : cr;
		COLORREF clr = dc.SetTextColor(cr);
		dc.DrawText ( &wch, 1, const_cast<CRect&>(rc), uFormat );
		dc.SetTextColor(clr);
	}

	void DrawTriangle(CDCHandle dc, int square)
	{
		const CRect rc = GetRect(square);
		Direction dir = m_board.GetDirection(square);
		if (dir != direction::none) DrawTriangle(dc, rc, dir);
	}

	// Set board client area
	void SetRect(const CRect &rcClient)
	{
		m_rect = rcClient;
	}

	// Get rectangle for specified square
	CRect GetRect(int square)
	{
		ATLASSERT(!m_rect.IsRectEmpty());
		int xorg = m_rect.left;
		int yorg = m_rect.top;
		int width = m_rect.Width();
		int height = m_rect.Height();
		int y = square / BoardWidth;
		int x = square % BoardWidth;
		int top = yorg + height * y / BoardWidth;
		int bottom = yorg + height * (y+1) / BoardWidth - 1;
		int left = xorg + width * x / BoardWidth;
		int right = xorg + width * (x+1) / BoardWidth - 1;
		return CRect(left,top,right,bottom);
	}

	void DrawTriangle(CDCHandle dc, CRect rc, Direction dir)
	{
		dc.SaveDC();
		int centerX = rc.Width()/2;
		int centerY = rc.Height()/2;

		CPoint edges[] = { CPoint(-1,-1),CPoint(0,-1),CPoint(1,-1), 
						  CPoint(-1,0),CPoint(1,0), 
						  CPoint(-1,1),CPoint(0,1),CPoint(1,1) };
//		CPoint triangle[] = {CPoint(-5,-5),CPoint(0,5),CPoint(5,-5)};
		CTriangle triangle = m_triangles[dir];
		for (int i=0; i<triangle.size(); i++)
		{
			CPoint edge = edges[dir];
			CPoint* ts = triangle;
			ts[i] += rc.TopLeft() + CPoint((edge.x+1)*centerX,(edge.y+1)*centerY);
		}
		CBrush red;
		red.CreateSolidBrush(COLORREF(0x000000cc));
		CBrushHandle prevBrush = dc.SelectBrush(red);
		dc.Polygon(triangle,triangle.size());
		dc.SelectBrush(prevBrush);
		dc.RestoreDC(-1);
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


class CBoardViewCtrl : public CBoardViewImpl<CBoardViewCtrl>
{
	typedef CBoardViewImpl<CBoardViewCtrl> baseClass;
public:
	CBoardViewCtrl(CBoard* pb) : baseClass(pb) {}
	DECLARE_WND_SUPERCLASS(_T("BoardViewCtrl"), GetWndClassName())
};

