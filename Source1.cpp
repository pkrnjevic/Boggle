#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

typedef std::list<int> CWord;
typedef std::deque<CWord> CWordList;

class CDict : public std::vector<CDict *>
{
public:
	typedef CDict* Ptr;
public:
	Ptr m_dict['z'-'a'+1];
	bool m_end_of_word;

	CDict(void) : m_end_of_word(false)
	{
		memset(m_dict,0,sizeof(m_dict));
//		this->resize('z'-'a'+1,0);
	}

	~CDict(void)
	{
	}

	CDict::Ptr& operator[](int n)
	{
		ATLASSERT(n >= 0 && n <= ('z'-'a'));
		return m_dict[n];
	}
	//void operator=(const CDict& dict)
	//{
	//	memcpy(this,&dict,sizeof(CDict));
	//}
	void operator=(CDict::Ptr pd)
	{
		ATLASSERT(pd);
		memcpy(this,pd,sizeof(CDict));
	}
	bool operator!()
	{
		return this == 0;
	}
	void Load(const std::wstring fn)
	{
		std::string line;

		std::ifstream myfile(CW2A(fn.c_str()));
		if (!myfile.is_open())
		{
			std::cout << "Unable to open file";
			return;
		}
		while (!myfile.eof()) {
			std::getline(myfile,line);
//			std::wcout << line << std::endl;
			Insert(line.c_str());
		}
	}
	bool EndOfWord()
	{
		return m_end_of_word;
	}
	CDict::Ptr Insert(std::string s)
	{
		return CDict::insert(this,s.c_str());
	}
	void Dump()
	{
		std::wstring s;
		CDict::dump(this,s);
	}
	bool Lookup(const std::wstring& s)
	{
		std::wstring::const_iterator i;
		CDict::Ptr pd = this;
		for (i=s.begin(); i!=s.end(); i++)
		{
			pd = pd->Lookup(*i);
			if (!pd) return false;
		}
		return pd->EndOfWord();
	}
	CDict::Ptr Lookup(wchar_t ch)
	{
		int i = CDict::to_n(ch);
		return (m_dict)[i];
	}
private:
	void setEndOfWord()
	{
		m_end_of_word = true;
	}
private:
	static bool notEmpty(CDict::Ptr dict)
	{
		return 0 != dict;
	}
	static CDict::Ptr insert(CDict::Ptr dict, const char* ps)
	{
		if (!ps || !*ps) 
		{
			ATLASSERT(dict);
			dict->setEndOfWord();
			return 0;
		}
		int i = to_n(*ps);
		if (i < 0 || i > 'z'-'a') 
		{
			std::wstring s = L"Invalid input in wordlist: " + s;
			OutputDebugString(s.c_str());
			return 0;
		}
		if (!(*dict)[i]) 
		{
			*(*dict)[i] = new CDict();
		}
		return insert((*dict)[i],++ps);
	}
	static bool dump(CDict::Ptr dict,std::wstring s)
	{
		if (!dict) return false;
		bool found = false;
		for (wchar_t ch='a'; ch<='z'; ch++)
		{
			int i = to_n(ch);
			std::wstring s1;
			s1.append(1,ch);
			found |= dump((*dict)[i], s+s1);
		}
		if (!found) {
			OutputDebugString(s.c_str());
			OutputDebugString(L"\n");
		}
		return true;
	}
	static int to_n(char ch)
	{
		ATLASSERT(ch >= 'a' && ch <= 'z');
		return ch - 'a';
	}
	static int to_n(const std::wstring s)
	{
		return to_n(s[0]);
	}
	static wchar_t to_ch(int n)
	{
		wchar_t ch = n + 'a';
		ATLASSERT(ch >= 'a' && ch <= 'z');
		return ch;
	}
};
