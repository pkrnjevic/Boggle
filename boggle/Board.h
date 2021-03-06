#pragma once

#define __BOARD_H__

#ifndef BOOST_SMART_ENUM_HPP_INCLUDED
  #error Board.h requires boost/smart_enum.hpp to be included first
#endif

#ifndef BOOST_BIND_HPP_INCLUDED
  #error Board.h requires boost/bind.hpp to be included first
#endif

#ifndef BOOST_SIGNAL_HPP
  #error Board.h requires boost/signal to be included first
#endif

#ifndef _OSTREAM_
  #error Board.h requires ostream to be included first
#endif

#ifndef _LIST_
  #error Board.h requires list to be included first
#endif

static const int BoardWidth = 5;
static const int BoardSize = BoardWidth * BoardWidth;

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

typedef std::wstring CWord;	
typedef std::deque<CWord> CWordList;	
typedef std::list<int> CSquares;

class CBoard
{
public:
	enum state {new_board, solved, new_word};
	typedef boost::signal<void (state)>  signal_t;
	typedef boost::signals::connection  connection_t;

private:
	std::wstring m_letters;
	CWordList m_wordlist;
	CWordList m_user_wordlist;
	CSquares m_selection;
	CDict& m_dict;
	signal_t m_sig;

public:

	CBoard(CDict::Ptr pd) : m_dict(*pd)
	{
		m_letters.resize(BoardSize);
	}

	~CBoard(void)
	{
	}

	// NB: not entirely const: the m_sig is modified as subscribers are added
    connection_t connect(signal_t::slot_function_type subscriber) const
    {
        return const_cast<CBoard*>(this)->m_sig.connect(subscriber);
    }

    void disconnect(connection_t subscriber)
    {
        subscriber.disconnect();
    }

	void Load(std::wstring s)
	{
		//std::wstring& rs = this;
		//rs = s;
		m_letters.assign(s);
		m_sig(new_board);
	}

	//std::wstring Entries(const CWord& v)
	//{
	//	std::wstring s;
	//	for (CWord::const_iterator i=v.begin(); i!=v.end(); i++)
	//	{
	//		s.append(1,m_letters[*i]);
	//	}
	//	return s;
	//}
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
		CSquares word=*(std::auto_ptr<CSquares>(new CSquares)), bool print=false)
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
//			OutputDebugString(s.c_str());
			wordlist.push_back(Word2String(word));
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
		m_selection = item < 0 ? CSquares() : String2Word(this->m_wordlist[item]);
	}

	void SetSelection(const CWord& w)
	{
		m_selection = String2Word(w);
	}

	//CSquares GetSelection() const
	//{
	//	return m_selection;
	//}

	std::wstring Word2String(const CSquares& w) const
	{
		std::wstring s;
		CSquares::const_iterator i;
		for (i=w.begin(); i!=w.end(); i++)
			s += this->m_letters[*i];
		return s;
	}

//	for each square on board
//		if matches first letter in string
//			drop first letter, test rest of string
	CSquares String2Word(const std::wstring& s) const
	{
		CSquares word;
		for (int i=0; i<BoardSize; i++)
		{
			if (s[0] == m_letters[i])
			{
				word.push_back(i);
				if (string2word(i,s.substr(1),word))
					break;
				word.pop_back();
			}
		}
		return word;
	}

//	do with rest of string
//		for each neighbour, test if it matches first letter
//			if so and letter hasn't been used yet
//				add square to CWord
//				if no more letters, done
//				else drop first letter and recurse
	bool string2word(int j, const std::wstring& s, CSquares& word) const
	{
		CIndex index(j);
		for (Direction dir=northwest; dir!=none; dir++)
		{
			if (s.empty())
				return true;
			int i = index.GetNeighbour(dir);
			ATLASSERT(i>=-1 && i<BoardSize);
			if (i != -1 && s[0] == m_letters[i])
			{
				if (word.end() != find(word.begin(),word.end(),i))
					continue;
				word.push_back(i);
				if (string2word(i,s.substr(1),word))
					return true;
				word.pop_back();
			}
		}
		return false;
	}

	bool Lookup(const std::wstring& s) const
	{
		return m_dict.Lookup(s);
	}

	int Find(const std::wstring& s) const
	{
		int i = 0;
		for (CWordList::const_iterator it=m_wordlist.begin(); it!=m_wordlist.end(); it++,i++)
		{
			if (wordEqual(s,*it))
				return i;
		}
		return -1;
	}

//	const CWordList::const_iterator& Find(const CWord& s) const
//	{
//		return std::find_if(m_wordlist.begin(), m_wordlist.end(), boost::bind(&CBoard::wordEqual,this,_1,s));
//	}

	const CWordList& Solve()
	{
	 	m_user_wordlist.clear();
	 	m_wordlist.clear();
		for (int i=0; i<BoardSize; i++)
			Walk(i, &m_dict, m_wordlist);
		std::sort(m_wordlist.begin(),m_wordlist.end(),boost::bind(&CBoard::wordCompare,this,_1,_2));
		m_sig(solved);
		return m_wordlist;
	}

	const CWordList& GetWordList() const
	{
		return m_wordlist;
	}

	const CWordList& GetUserWordList() const
	{
		return m_user_wordlist;
	}

	void AddUserWord(const CSquares& w)
	{
		m_user_wordlist.push_back(Word2String(w));
		m_sig(new_word);
	}

	bool IsSquareUsed(int square)
	{
		bool found = false;
		if (!m_selection.empty())
		{
			const CSquares& v = this->m_selection;
			CSquares::const_iterator i = std::find(v.begin(), v.end(), square);
			found = v.end() != i;
		}
		return found;
	}

	// Get direction to next letter in word (used to draw arrows)
	Direction GetDirection(int square)
	{
		Direction dir = direction::none;
		bool found = false;
		if (!m_selection.empty())
		{
			const CSquares& v = m_selection;
			CSquares::const_iterator i = std::find(v.begin(), v.end(), square);
			found = v.end() != i;
			if (found)
			{
				const CSquares::const_iterator next = ++i;
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

	bool wordEqual( const CWord& a, const CWord& b ) const
	{
		return a == b;
	}

	bool wordCompare( const CWord& a, const CWord& b ) const
	{
		return a < b;
	}

};
