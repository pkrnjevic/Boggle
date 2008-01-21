#pragma once

#define __BOARD_H__

//
//class Index
//    @@neighbours = [-(Width+1),-Width,-(Width-1),-1,1,(Width-1),Width,(Width+1)]
//    def initialize(n)
//        @n = n
//    end
//    def Index.to_xy(n)
//        return n%Width,n/Width
//    end
//    def Index.valid?(n)
//        x,y = Index.to_xy(n)
//        return valid_point?(x,y)
//    end
//    def get_neighbour(n)
//        x = @n%Width
//        case @@neighbours[n]
//        when -(Width+1), -1, (Width-1):
//            return nil if x == 0
//        when -(Width-1), 1, (Width+1):
//            return nil if x == (Width-1)
//        end
//        neighbour = @n + @@neighbours[n]
//        return nil if !Index.valid?(neighbour)
//        return neighbour
//    end
//end
//
//class Board < Array
//    def load(a)
//        puts 'bad input length' if a.length > BoardSize
//        i = 0
//        BoardSize.times do |i|
//            self[i] = a[i]
//        end
//    end
//    def valid?(x,y)
//        Point.valid?(x,y)
//    end
//    def entries(l)
//        a = []
//        l.each {|i| a << self[i..i]}
//        return a
//    end
//    def to_s(n = :all)
//        return all_to_s if n == :all
//        row_to_s(n)
//    end
//  private
//    def row_to_s(n)
//        a = []
//        i = n * Width
//        Width.times {|j| a << self[i+j]}
//        return a.to_s
//    end
//    def all_to_s
//        as = []
//        Width.times do |i|
//            as << row_to_s(i)
//        end
//        return as
//    end
//end
//
//class Index
//    @@neighbours = [-(Width+1),-Width,-(Width-1),-1,1,(Width-1),Width,(Width+1)]
//    def initialize(n)
//        @n = n
//    end
//    def Index.to_xy(n)
//        return n%Width,n/Width
//    end
//    def Index.valid?(n)
//        x,y = Index.to_xy(n)
//        return valid_point?(x,y)
//    end
//    def get_neighbour(n)
//        x = @n%Width
//        case @@neighbours[n]
//        when -(Width+1), -1, (Width-1):
//            return nil if x == 0
//        when -(Width-1), 1, (Width+1):
//            return nil if x == (Width-1)
//        end
//        neighbour = @n + @@neighbours[n]
//        return nil if !Index.valid?(neighbour)
//        return neighbour
//    end
//end
//

#define Width 5
#define BoardSize (Width*Width)

class CBoard
{
public:

	CBoard(void)
	{
	}

	~CBoard(void)
	{
	}
};
