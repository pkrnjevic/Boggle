#pragma once

#define __BOARD_H__

#include <boost/smart_enum.hpp>
#include <ostream>
#include <list>

#define BoardWidth 5
#define BoardSize (BoardWidth*BoardWidth)

typedef std::list<int> CVisited;
typedef std::deque<CVisited> CVisitList;

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
		for (Direction dir=northwest; dir!=none; dir++)
		{
			int n = index.GetNeighbour(dir);
			if (n != -1)
				print = Walk(n,dict,visitlist,visited,print);
		}
		return print;
	}

	~CBoard(void)
	{
	}
};
