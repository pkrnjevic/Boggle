#!/usr/bin/env ruby

require 'pp'

Width = 5
BoardSize = Width * Width
#Dictionary = 'wordlist'
Dictionary = 'word.list'

# Letter distribution on Boggle dice (4x4 and 5x5)
# aaaaacdddeeeeehh    aaaaaaaabccccddddeeefgino
# abcfoieeieehilil    fdeaeaeajceeehhdhnmiiopoo
# ebhfomilsgirormn    ieeageefkeiiillhhsoiprroo
# ejoktolrthntstnn    rnefmegiqnlipnnnlstirrrtt
# goppttrvtnsvstqr    snernemrxsplsoooosttsvrut
# nosswuxyywuwtyuz    ynmsneuszttttrrtruttywywu

def roll_dice
    faces = []
    if BoardSize == 16
        faces[0] = 'aaaaacdddeeeeehh'
        faces[1] = 'abcfoieeieehilil'
        faces[2] = 'ebhfomilsgirormn'
        faces[3] = 'ejoktolrthntstnn'
        faces[4] = 'goppttrvtnsvstqr'
        faces[5] = 'nosswuxyywuwtyuz'
    else
        faces[0] = 'aaaaaaaabccccddddeeefgino'
        faces[1] = 'fdeaeaeajceeehhdhnmiiopoo'
        faces[2] = 'ieeageefkeiiillhhsoiprroo'
        faces[3] = 'rnefmegiqnlipnnnlstirrrtt'
        faces[4] = 'snernemrxsplsoooosttsvrut'
        faces[5] = 'ynmsneuszttttrrtruttywywu'
    end
    result = []
    BoardSize.times {|i| result[i] = faces[rand_int(6)][i..i]}
    shuffle(result)
end

def rand_int(max)
    rand()*max.to_int
end

def shuffle(a)
    n = a.length
    n.times {|i| j=rand_int(n); v=a[i]; a[i]=a[j]; a[j]=v}
    a
end

def valid_point?(x,y)
    return false if x < 0 || x >= Width
    return false if y < 0 || y >= Width
    return true
end

def has_upper(string)
    string =~ /[A-Z]/
end

class Index
    @@neighbours = [-(Width+1),-Width,-(Width-1),-1,1,(Width-1),Width,(Width+1)]
    def initialize(n)
        @n = n
    end
    def Index.to_xy(n)
        return n%Width,n/Width
    end
    def Index.valid?(n)
        x,y = Index.to_xy(n)
        return valid_point?(x,y)
    end
    def get_neighbour(n)
        x = @n%Width
        case @@neighbours[n]
        when -(Width+1), -1, (Width-1):
            return nil if x == 0
        when -(Width-1), 1, (Width+1):
            return nil if x == (Width-1)
        end
        neighbour = @n + @@neighbours[n]
        return nil if !Index.valid?(neighbour)
        return neighbour
    end
end

class Board < Array
    def load(a)
        puts 'bad input length' if a.length > BoardSize
        i = 0
        BoardSize.times do |i|
            self[i] = a[i]
        end
    end
    def valid?(x,y)
        Point.valid?(x,y)
    end
    def entries(l)
        a = []
        l.each {|i| a << self[i..i]}
        return a
    end
    def to_s(n = :all)
        return all_to_s if n == :all
        row_to_s(n)
    end
  private
    def row_to_s(n)
        a = []
        i = n * Width
        Width.times {|j| a << self[i+j]}
        return a.to_s
    end
    def all_to_s
        as = []
        Width.times do |i|
            as << row_to_s(i)
        end
        return as
    end
end

class Dict < Array
    def insert(s)
        Dict.insert_dict(self,s)
    end
    def dump
#        pp self
        Dict.dump_dict(self)
    end
    # Don't use this! YAWL is better
    # process input from ispell 
    def build
        a = ''
        Dir.foreach('dict') do |f|
            next if f[0..0] == '.'
            a << File.read('dict/'+f)
        end
        b = a.split
        b.map! {|e| e.gsub(/(.*)'./,'\1')}      # get rid of apostrophe 
        b.map! {|e| e.gsub(/'/,'')}             # get rid of more apostrophe
#        b.map! {|e| e.downcase}
        d = []
        b.each do |e|
            if has_upper(e)
                puts "skipping '#{e}' due to capitalization"
            else
                d << e
            end
        end
        c = d.sort.uniq
        File.open(Dictionary,"w") do |f|
            c.each {|line| f.puts(line)}
        end
    end
    def load
        c = []
        # must be a better way - but crlf handling is weird in windows
        # tried gets, stripping \n with gsub, substring: all slower
        a = File.read(Dictionary)
        c = a.split
        c.each {|e| self.insert(e)}
    end
    def load1
        a = ''
        Dir.foreach('dict') do |f|
            next if f[0..0] == '.'
            a << File.read('dict/'+f)
        end
        b = a.split
        b.map! {|e| e.gsub(/(.*)'./,'\1')}      # get rid of apostrophe 
        b.map! {|e| e.gsub(/'/,'')}             # get rid of more apostrophe 
        c = b.sort.uniq
        c.each {|e| self.insert(e)}
    end
    def lookup(ch)
        i = Dict.to_n(ch)
#        pp ch, i
#        p self
#        return self
        return self[i]
    end
    def end_of_word?
        empty?
    end
  private
    def Dict.insert_dict(dict,s)
        return if s.length == 0
        i = to_n(s)
        if i < 0 || i > 'z'[0]	# bug: to_n('z')
            puts "insert dict error: invalid char val=#{i+'a'[0]}"
            puts "debug = '#{s}'"
            return
        end
        dict[i] = Dict.new if !dict[i]
        insert_dict(dict[i],s[1..-1])
    end
    def Dict.dump_dict(dict,s='')
        return false if !dict
        found = false
        ('a'..'z').each do |ch|
            i = to_n(ch)
            found |= dump_dict(dict[i],s+ch)
        end
        puts s if !found
        return true
    end
    # convert first char in string to
    # integer where 'a'=0, 'b'=1, etc
    def Dict.to_n(ch)
        ch[0]-'a'[0]
    end
    def Dict.to_ch(n)
        ('a'+n).chr
    end
end

def visit_neighbours(j,visited,print=false,dict=$dict,board=$board)
    trace = false
    return false if !dict
    if visited.include?(j)
        puts board.entries(visited).to_s if print && trace
        return false
    end
    dict = dict.lookup(board[j])
    if !dict
        puts board.entries(visited).to_s if print && trace
        return false
    end
    visited << j
    if dict.end_of_word?
        print board.entries(visited).to_s
        print ' '
        pp visited
    end
    print = true
    if visited.length == BoardSize
        puts board.entries(visited).to_s if print && trace
        return false
    end
#    pp visited
    index = Index.new(j)
    8.times do |i|
        n = index.get_neighbour(i)
        if n
            print = visit_neighbours(n,visited.dup,print,dict,board)
        end
    end
    return print
end

# Simple dict tests
#$dict = Dict.new
#$dict.insert('abcd')
#$dict.insert('foo')
#$dict.insert('yuck')
#$dict.dump

# Dictionary stuff
$dict = Dict.new
# Don't use build - YAWL list supersedes ispell
#$dict.build if !File.exists?(Dictionary)
$dict.load
#$dict.dump

# Board stuff
$board = Board.new
#a = 'fdeaeaeajceeehhdhnmiiopoo' if Width == 5
#a = 'abcdfoodyuckzyxv' if Width == 4
#a = 'abcdefghijklmnop'
#$board.load(a.split(//))
$board.load(roll_dice())
puts $board.to_s
puts

# Walk the board
BoardSize.times {|i| visit_neighbours(i,[])}
#1.times {|i| visit_neighbours(i,visited.dup)}

