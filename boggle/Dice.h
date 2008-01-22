#pragma once

#include <string>
#include <time.h>

#ifndef __BOARD_H__
  #error Dice.h requires Board.h to be included first
#endif

class CDice
{
public:
	std::string faces[6];

	CDice(void)
	{
#if Width == 4
        faces[0] = "aaaaacdddeeeeehh";
        faces[1] = "abcfoieeieehilil";
        faces[2] = "ebhfomilsgirormn";
        faces[3] = "ejoktolrthntstnn";
        faces[4] = "goppttrvtnsvstqr";
        faces[5] = "nosswuxyywuwtyuz";
#else 
        faces[0] = "aaaaaaaabccccddddeeefgino";
        faces[1] = "fdeaeaeajceeehhdhnmiiopoo";
        faces[2] = "ieeageefkeiiillhhsoiprroo";
        faces[3] = "rnefmegiqnlipnnnlstirrrtt";
        faces[4] = "snernemrxsplsoooosttsvrut";
        faces[5] = "ynmsneuszttttrrtruttywywu";	
#endif
		srand ( (unsigned int)time(NULL) );
	}

	std::wstring RollDice()
	{
		std::wstring dice = L"abcdefghijklmnopqrstuvwxyz";
		dice.reserve(BoardSize);

		// initialize dice
		for (int i=0; i<BoardSize; i++)
		{
			int j = rand() % 6;
			dice[i] = faces[j][i];
		}
		// shuffle dice
		for (int i=0; i<BoardSize; i++)
		{
			int j = rand() %  BoardSize;
			wchar_t val = dice[i];
			dice[i] = dice[j];
			dice[j] = val;
		}
		return dice;
	}

	~CDice(void)
	{
	}
};
