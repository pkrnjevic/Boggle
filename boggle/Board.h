#pragma once

#define __BOARD_H__

#include <ostream>
#include <list>

#define Width 5
#define BoardSize (Width*Width)

typedef std::list<int> CVisited;
typedef std::deque<CVisited> CVisitList;

static const int neighbour_list[8] = 
	{-(Width+1),-Width,-(Width-1),-1,1,(Width-1),Width,(Width+1)};

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
		int x = n % Width;
		int y = n / Width;
		return std::pair<int,int>(x,y);
	}
	bool isValid(int n)
	{
		return n >= 0 && n < BoardSize;
	}
	int GetNeighbour(int n)
	{
		ATLASSERT(n >= 0 && n < 8);
		int x = m_n % Width;
		switch (neighbour_list[n]) {
		case -(Width+1):
		case -1:
		case Width-1:
			if (!x) return -1;
			break;
		case -(Width-1):
		case 1:
		case Width+1:
			if (x == Width-1) return -1;
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
			os << L"<TABLE STYLE=\"table-layout:fixed\">";
			for (int i=0; i<Width; i++)
				os << L"<COL WIDTH=30>";
			for (int i=0; i<Width; i++)
				os << this->Format(i,visited);
			os << L"</TABLE>";
		}
		else
		{
			ATLASSERT(row >= 0 && row < Width);
			int j = row * Width;
			os << L"<TR>";
			for (int i=0; i<Width; i++)
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
