#pragma once

#define __BOARD_H__

#include <ostream>
#include <list>

#define BoardWidth 5
#define BoardSize (BoardWidth*BoardWidth)

typedef std::list<int> CVisited;
typedef std::deque<CVisited> CVisitList;

static const int neighbour_list[8] = 
	{-(BoardWidth+1),-BoardWidth,-(BoardWidth-1),-1,1,(BoardWidth-1),BoardWidth,(BoardWidth+1)};

class CIndex
{
	int m_n;
public:
	CIndex(int n)
	{
		m_n = n;
	}
	std::pair<int,int> toXY(int n)
	{
		int x = n % BoardWidth;
		int y = n / BoardWidth;
		return std::pair<int,int>(x,y);
	}
	bool isValid(int n)
	{
		return n >= 0 && n < BoardSize;
	}
	int GetNeighbour(int n)
	{
		ATLASSERT(n >= 0 && n < 8);
		int x = m_n % BoardWidth;
		switch (neighbour_list[n]) {
		case -(BoardWidth+1):
		case -1:
		case BoardWidth-1:
			if (!x) return -1;
			break;
		case -(BoardWidth-1):
		case 1:
		case BoardWidth+1:
			if (x == BoardWidth-1) return -1;
			break;
		};
		int neighbour = m_n + neighbour_list[n];
		if (isValid(neighbour))
			return neighbour;
		else
			return -1;
	}

};

class CBoard : public std::wstring
{
public:

	CBoard(void)
	{
		this->reserve(BoardSize);
	}

	void Load(std::wstring s)
	{
		//std::wstring& rs = this;
		//rs = s;
		this->assign(s);
	}

	std::wstring Entries(CVisited v)
	{
		std::wstring s;
		for (CVisited::iterator i=v.begin(); i!=v.end(); i++)
		{
			s.append(1,(*this)[*i]);
		}
		return s;
	}
	std::wstring Format(int row = -1, CVisited* visited=NULL)
	{
		std::wostringstream os;
		if (row == -1)
		{
			for (int i=0; i<BoardWidth; i++)
				os << this->Format(i,visited);         // << std::endl;
		}
		else
		{
			ATLASSERT(row >= 0 && row < BoardWidth);
			int j = row * BoardWidth;
			for (int i=0; i<BoardWidth; i++)
			{
				bool highlight = visited == NULL ? false : 
					visited->end() != std::find(visited->begin(),visited->end(),j);
				if (highlight)
					os << char((*this)[j++] - 'a'+'A');
				else
					os << char((*this)[j++] - 'a'+'A');
			}
		}
		return os.str();
	}
	std::wstring FormatHTML(int row = -1, CVisited* visited=NULL)
	{
		std::wostringstream os;
		if (row == -1)
		{
			os << L"<TABLE STYLE=\"table-layout:fixed\">";
			for (int i=0; i<BoardWidth; i++)
				os << L"<COL WIDTH=30>";
			for (int i=0; i<BoardWidth; i++)
				os << this->Format(i,visited);
			os << L"</TABLE>";
		}
		else
		{
			ATLASSERT(row >= 0 && row < BoardWidth);
			int j = row * BoardWidth;
			os << L"<TR>";
			for (int i=0; i<BoardWidth; i++)
			{
				bool highlight = visited == NULL ? false : 
					visited->end() != std::find(visited->begin(),visited->end(),j);
				if (highlight)
					os << L"<TD><B><font color=\"#ff0000\">" << char((*this)[j++] - 'a'+'A') << L"</font></B></TD>";
				else
					os << L"<TD>" << char((*this)[j++] - 'a'+'A') << L"</TD>";
			}
			os << L"</TR>";
		}
		return os.str();
	}

	bool Walk(int j, CDictPtr dict, CVisitList& visitlist, 
		CVisited visited=*(std::auto_ptr<CVisited>(new CVisited)), bool print=false)
	{
		if (!dict) return false;
		if (visited.end() != std::find(visited.begin(),visited.end(),j)) 
		{
			// print?
			return false;
		}
		dict = dict->Lookup((*this)[j]);
		if (!dict) 
		{
			// print?
			return false;
		}
		visited.push_back(j);
		if (dict->EndOfWord())
		{
			visitlist.push_back(visited);
		}
		print = true;
		if (visited.size() == BoardSize)
		{
			// print?
			return false;
		}
		CIndex index(j);
		for (int i=0; i<8; i++)
		{
			int n = index.GetNeighbour(i);
			if (n != -1)
				print = Walk(n,dict,visitlist,visited,print);
		}
		return print;
	}

	~CBoard(void)
	{
	}
};


class CBoardView : public CStatic
{
public:
	CFont m_font;
	CBoardView() : m_font(0)
	{
	}
	void Init()
	{
		CSize size(0,0);
		this->OnSize(0,size);
	}

	void OnSize(UINT nType, CSize /*size*/)
	{
		if (!m_hWnd)
			return;

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
		this->GetClientRect(rc);
		lf.lfHeight = rc.Height() / 7;
//		lf.lfWidth = rc.Width() / 7;
		m_font.CreateFontIndirect ( &lf ); 		
		//			this->Invalidate(false);
	}

	void DrawChar(CDCHandle dc, const CRect& rc, char ch)
	{
		const UINT uFormat = DT_CENTER|DT_VCENTER|DT_NOPREFIX;
		wchar_t s[2]=L"";
		s[0] = ch;
		dc.FrameRect(rc,(HBRUSH)::GetStockObject(BLACK_BRUSH));
		dc.DrawText ( CT2CW(s), -1, const_cast<CRect&>(rc), uFormat );
	}

	void Draw(CDCHandle dc, const CRect& rcCtrl)
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

			int xorg = rcCtrl.left;
			int yorg = rcCtrl.top;
			int width = rcCtrl.Width();
			int height = rcCtrl.Height();
			int i = 0;
			for (int y=0; y<BoardWidth; y++) 
			{
				int top = yorg + height * y / BoardWidth;
				int bottom = yorg + height * (y+1) / BoardWidth;
				for (int x=0; x<BoardWidth; x++,i++)
				{
					int left = xorg + width * x / BoardWidth;
					int right = xorg + width * (x+1) / BoardWidth;
					CRect rc(left,top,right,bottom);
					DrawChar(dc, rc, sText[i]);
				}
			}
//			dc.DrawText ( CT2CW(sText), -1, const_cast<CRect&>(rcCtrl), uFormat );

			dc.RestoreDC(-1);
//		}
	}
};