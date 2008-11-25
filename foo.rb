#/usr/bin/ruby

require 'pp'

def Radians(degrees)
  pi = 3.141592653589739323846
  return degrees / (180.0 / pi)
end

x = 100.0
y = 100.0
angle = Radians(45.0)
xx = x*Math.cos(angle)-y*Math.sin(angle)
yy = x*Math.sin(angle)+y*Math.cos(angle)
pp x,y,xx,yy

