// StringUtils.h

#pragma once

#define __STRING_UTILS_H__

#include < string >
#include < algorithm >
#include < cctype >

namespace StringUtils
{
	void MakeUpper(std::string &str)
	{
		std::transform(str.begin(),str.end(),str.begin(),toupper);
	}

	void MakeLower(std::string &str)
	{
		std::transform(str.begin(),str.end(),str.begin(),tolower);
	}

	void TrimLeft(std::string& str, const char* chars2remove)
	{
		if (!str.empty())
		{
			std::string::size_type pos = str.find_first_not_of(chars2remove);

			if (pos != std::string::npos)
				str.erase(0,pos);
			else
				str.erase( str.begin() , str.end() ); // make empty
		}
	}

	void TrimRight(std::string& str, const char* chars2remove)
	{
		if (!str.empty())
		{
			std::string::size_type pos = str.find_last_not_of(chars2remove);

			if (pos != std::string::npos)
				str.erase(pos+1);
			else
				str.erase( str.begin() , str.end() ); // make empty
		}
	}


	void ReplaceLeading(std::string& str, const char* chars2replace, char c)
	{
		if (!str.empty())
		{
			std::string::size_type pos = str.find_first_not_of(chars2replace);

			if (pos != std::string::npos)
				str.replace(0,pos,pos,c);
			else
			{
				int n = str.size();
				str.replace(str.begin(),str.end()-1,n-1,c);
			}
		}
	}

}

//// testing code
//
//#include < iostream >
//#include < string >
//
//#include "StringUtils.h"
//
//int main()
//{
//	using namespace StringUtils;
//
//	using std::cout;
//	using std::endl;
//	using std::string;
//
//	string str;
//
//	str = " abc";
//	TrimLeft(str); // remove leading blanks
//	cout << str << endl;
//
//	str = "ssss00 00";
//	TrimRight(str,"0 "); // remove trailing zeroes and blanks
//	cout << str << endl;
//
//	str = " 12340";
//	ReplaceLeading(str," ",'*'); // replace leading blanks with *
//	cout << str << endl;
//
//	str = " 12340";
//	ReplaceLeading(str," ",'0'); // replace leading blanks with zeroes
//	cout << str << endl;
//
//	str = "Some String";
//	MakeUpper(str); // make uppercase
//	cout << str << endl;
//
//	MakeLower(str); // make lowercase
//	cout << str << endl;
//
//	return 0;
//}
//