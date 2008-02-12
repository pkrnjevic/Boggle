#pragma once

#define __BOARD_H__

#include <boost/smart_enum.hpp>
#include <boost/bind.hpp>
#include <ostream>
#include <list>

#define BoardWidth 5
#define BoardSize (BoardWidth*BoardWidth)

static const int neighbour_list[8] = 
	{-(BoardWidth+1),-BoardWidth,-(BoardWidth-1),-1,1,(BoardWidth-1),BoardWidth,(BoardWidth+1)};

enum direction { northwest, north, northeast, west, east, southwest, south, southeast, none };
typedef  boost::sequential_smart_enum<direction,northwest,none> Direction;

class CIndex
{
	int m_n;
public:
	CIndex(int n)
	{
		m_n = n;
	}
	bool isValid(int n)
	{
		return n >= 0 && n < BoardSize;
	}
	bool isNeighbour(int n)
	{
		if (!isValid(n)) return false;
		CPoint xy(toXY(m_n));
		CPoint nxy(toXY(n));
		CPoint diff(nxy-xy);
		return isNeighbour(diff);
	}
	Direction GetNeighbourIndex(int n)
	{
		CPoint xy(toXY(m_n));
		CPoint nxy(toXY(n));
		CPoint diff(nxy-xy);
		return GetNeighbourIndex(diff);
	}
	int GetNeighbour(Direction dir)
	{
//		ATLASSERT(n >= 0 && n < 9);
//		ATLASSERT(n);
		int neighbour = m_n + neighbour_list[dir];
		return isNeighbour(neighbour) ? neighbour : -1;
	}
private:
	CPoint toXY(int n)
	{
		int x = n % BoardWidth;
		int y = n / BoardWidth;
		return CPoint(x,y);
	}
	bool isNeighbour(CPoint xy)
	{
		if (xy.x < -1 || xy.x > 1) return false;
		if (xy.y < -1 || xy.y > 1) return false;
		if (xy.x == 0 && xy.y == 0) return false;
		return true;
	}
	Direction GetNeighbourIndex(CPoint xy)
	{
		Direction rv(direction::none);
		if (!isNeighbour(xy)) return rv;
		if (xy.y < 0) rv = direction::north + xy.x;
		else if (xy.y == 0 && xy.x == -1) rv = direction::west;
		else if (xy.y == 0 && xy.x == 1) rv = direction::east;
		else if (xy.y > 0) rv = direction::south + xy.x;
		else rv = none;
		return rv;
	}
};

class CBoard
{
	 std::wstring m_letters;
	 CWordList m_wordlist;
	 CDict& m_dict;
	 int m_selection;

public:

	CBoard(CDict::Ptr pd) : m_dict(*pd), m_selection(-1)
	{
		m_letters.resize(BoardSize);
	}

	~CBoard(void)
	{
	}

	void Load(std::wstring s)
	{
		//std::wstring& rs = this;
		//rs = s;
		m_letters.assign(s);
	}

	std::wstring Entries(CWord v)
	{
		std::wstring s;
		for (CWord::iterator i=v.begin(); i!=v.end(); i++)
		{
			s.append(1,m_letters[*i]);
		}
		return s;
	}
	std::wstring Format(int row = -1)
	{
		std::wostringstream os;
		if (row == -1)
		{
			for (int row=0; row<BoardWidth; row++)
				os << this->Format(row);		// << std::endl;
		}
		else
		{
			ATLASSERT(row >= 0 && row < BoardWidth);
			int j = row * BoardWidth;
			for (int i=0; i<BoardWidth; i++)
			{
//				std::find(word.begin(),word.end(),j);
				os << char(m_letters[j++] - 'a'+'A');
			}
		}
		return os.str();
	}

	//std::wstring FormatHTML(int row = -1, CWord* word=NULL)
	//{
	//	std::wostringstream os;
	//	if (row == -1)
	//	{
	//		os << L"<TABLE STYLE=\"table-layout:fixed\">";
	//		for (int i=0; i<BoardWidth; i++)
	//			os << L"<COL WIDTH=30>";
	//		for (int i=0; i<BoardWidth; i++)
	//			os << this->Format(i);
	//		os << L"</TABLE>";
	//	}
	//	else
	//	{
	//		ATLASSERT(row >= 0 && row < BoardWidth);
	//		int j = row * BoardWidth;
	//		os << L"<TR>";
	//		for (int i=0; i<BoardWidth; i++)
	//		{
	//			bool highlight = word == NULL ? false : 
	//				word->end() != std::find(word->begin(),word->end(),j);
	//			if (highlight)
	//				os << L"<TD><B><font color=\"#ff0000\">" << char(m_letters[j++] - 'a'+'A') << L"</font></B></TD>";
	//			else
	//				os << L"<TD>" << char(m_letters[j++] - 'a'+'A') << L"</TD>";
	//		}
	//		os << L"</TR>";
	//	}
	//	return os.str();
	//}

	bool Walk(int j, CDict::Ptr dict, CWordList& wordlist, 
		CWord word=*(std::auto_ptr<CWord>(new CWord)), bool print=false)
	{
		if (!dict) 
			return false;

		// don't allow squares to be used twice
		if (word.end() != std::find(word.begin(),word.end(),j)) 
			return false;

		dict = dict->Lookup(m_letters[j]);
		if (!dict) 
			return false;

		word.push_back(j);
		if (dict->EndOfWord())
		{
			std::wstring s;
			s = L"\n";
			s = Word2String(word);
			s += L"\n";
			OutputDebugString(s.c_str());
			wordlist.push_back(word);
		}

		print = true;
		if (word.size() == BoardSize)
			return false;

		CIndex index(j);
		for (Direction dir=northwest; dir!=none; dir++)
		{
			int n = index.GetNeighbour(dir);
			if (n != -1)
				print = Walk(n,dict,wordlist,word,print);
		}
		return print;
	}

	void SetSelection(int item)
	{
		ATLASSERT(item >= -1);
		m_selection = item;
	}

	int GetSelection()
	{
		return m_selection;
	}

	std::wstring Word2String(const CWord& w)
	{
		std::wstring s;
		CWord::const_iterator i;
		for (i=w.begin(); i!=w.end(); i++)
			s += this->m_letters[*i];
		return s;
	}

	bool Lookup(const std::wstring s)
	{
		return m_dict.Lookup(s);
	}

	const CWordList& Solve()
	{
	 	m_wordlist.clear();
		for (int i=0; i<BoardSize; i++)
			Walk(i, &m_dict, m_wordlist);
		std::sort(m_wordlist.begin(),m_wordlist.end(),boost::bind(&CBoard::wordCompare,this,_1,_2));
		return m_wordlist;
	}

	bool IsSquareUsed(int square)
	{
		bool found = false;
		if (m_selection != -1)
		{
			CWord& v = m_wordlist[m_selection];
			CWord::iterator i = std::find(v.begin(), v.end(), square);
			found = v.end() != i;
		}
		return found;
	}

	// Get direction to next letter in word
	Direction GetDirection(int square)
	{
		Direction dir = direction::none;
		bool found = false;
		if (m_selection != -1)
		{
			CWord& v = m_wordlist[m_selection];
			CWord::iterator i = std::find(v.begin(), v.end(), square);
			found = v.end() != i;
			if (found)
			{
				CWord::iterator next = ++i;
				if (next != v.end())
				{
					int next_square = *next;
					CIndex ndx(square);
					if (ndx.isNeighbour(next_square))
					{
						dir = ndx.GetNeighbourIndex(next_square);
						char s[1000];
						sprintf(s,"square=%d, next_square=%d, direction=%d\n",square,next_square,dir);
						OutputDebugStringA(s);
					}
				}
			}
		}
		return dir;
	}

private:

	bool wordCompare( const CWord& a, const CWord& b )
	{
		return Word2String(a) < Word2String(b);
	}

};
