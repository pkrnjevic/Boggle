#pragma once

//class Dict < Array
//    def insert(s)
//        Dict.insert_dict(self,s)
//    end
//    def dump
//#        pp self
//        Dict.dump_dict(self)
//    end
//    # Don't use this! YAWL is better
//    # process input from ispell 
//    def build
//        a = ''
//        Dir.foreach('dict') do |f|
//            next if f[0..0] == '.'
//            a << File.read('dict/'+f)
//        end
//        b = a.split
//        b.map! {|e| e.gsub(/(.*)'./,'\1')}      # get rid of apostrophe 
//        b.map! {|e| e.gsub(/'/,'')}             # get rid of more apostrophe
//#        b.map! {|e| e.downcase}
//        d = []
//        b.each do |e|
//            if has_upper(e)
//                puts "skipping '#{e}' due to capitalization"
//            else
//                d << e
//            end
//        end
//        c = d.sort.uniq
//        File.open(Dictionary,"w") do |f|
//            c.each {|line| f.puts(line)}
//        end
//    end
//    def load
//        c = []
//        # must be a better way - but crlf handling is weird in windows
//        # tried gets, stripping \n with gsub, substring: all slower
//        a = File.read(Dictionary)
//        c = a.split
//        c.each {|e| self.insert(e)}
//    end
//    def load1
//        a = ''
//        Dir.foreach('dict') do |f|
//            next if f[0..0] == '.'
//            a << File.read('dict/'+f)
//        end
//        b = a.split
//        b.map! {|e| e.gsub(/(.*)'./,'\1')}      # get rid of apostrophe 
//        b.map! {|e| e.gsub(/'/,'')}             # get rid of more apostrophe 
//        c = b.sort.uniq
//        c.each {|e| self.insert(e)}
//    end
//    def lookup(ch)
//        i = Dict.to_n(ch)
//#        pp ch, i
//#        p self
//#        return self
//        return self[i]
//    end
//    def end_of_word?
//        empty?
//    end
//  private
//    def Dict.insert_dict(dict,s)
//        return if s.length == 0
//        i = to_n(s)
//        if i < 0 || i > 'z'[0]
//            puts "insert dict error: invalid char val=#{i+'a'[0]}"
//            puts "debug = '#{s}'"
//            return
//        end
//        dict[i] = Dict.new if !dict[i]
//        insert_dict(dict[i],s[1..-1])
//    end
//    def Dict.dump_dict(dict,s='')
//        return false if !dict
//        found = false
//        ('a'..'z').each do |ch|
//            i = to_n(ch)
//            found |= dump_dict(dict[i],s+ch)
//        end
//        puts s if !found
//        return true
//    end
//    # convert first char in string to
//    # integer where 'a'=0, 'b'=1, etc
//    def Dict.to_n(ch)
//        ch[0]-'a'[0]
//    end
//    def Dict.to_ch(n)
//        ('a'+n).chr
//    end
//end


class CDict
{
public:

	CDict(void)
	{
	}

	~CDict(void)
	{
	}
};
