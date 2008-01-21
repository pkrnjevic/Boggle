#pragma once

#include <string>
#include <time.h>

#ifndef __BOARD_H__
  #error Dice.h requires Board.h to be included first
#endif

//# Letter distribution on Boggle dice (4x4 and 5x5)
//# aaaaacdddeeeeehh    aaaaaaaabccccddddeeefgino
//# abcfoieeieehilil    fdeaeaeajceeehhdhnmiiopoo
//# ebhfomilsgirormn    ieeageefkeiiillhhsoiprroo
//# ejoktolrthntstnn    rnefmegiqnlipnnnlstirrrtt
//# goppttrvtnsvstqr    snernemrxsplsoooosttsvrut
//# nosswuxyywuwtyuz    ynmsneuszttttrrtruttywywu
//
//def roll_dice
//    faces = []
//    if BoardSize == 16
//        faces[0] = 'aaaaacdddeeeeehh'
//        faces[1] = 'abcfoieeieehilil'
//        faces[2] = 'ebhfomilsgirormn'
//        faces[3] = 'ejoktolrthntstnn'
//        faces[4] = 'goppttrvtnsvstqr'
//        faces[5] = 'nosswuxyywuwtyuz'
//    else
//        faces[0] = 'aaaaaaaabccccddddeeefgino'
//        faces[1] = 'fdeaeaeajceeehhdhnmiiopoo'
//        faces[2] = 'ieeageefkeiiillhhsoiprroo'
//        faces[3] = 'rnefmegiqnlipnnnlstirrrtt'
//        faces[4] = 'snernemrxsplsoooosttsvrut'
//        faces[5] = 'ynmsneuszttttrrtruttywywu'
//    end
//    result = []
//    BoardSize.times {|i| result[i] = faces[rand_int(6)][i..i]}
//    shuffle(result)
//end
//
//def rand_int(max)
//    rand()*max.to_int
//end
//
//def shuffle(a)
//    n = a.length
//    n.times {|i| j=rand_int(n); v=a[i]; a[i]=a[j]; a[j]=v}
//    a
//end
//

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
			char val = dice[i];
			dice[i] = dice[j];
			dice[j] = val;
		}
		return dice;
	}

	~CDice(void)
	{
	}
};
