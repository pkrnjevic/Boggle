#!/usr/bin/ruby

a = File.read("word.list")
a.each {|e| puts e.downcase}
