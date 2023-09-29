#!/usr/bin/python

import sys
import py_compile

def func1():
  """
  the sneaky vicious fox pesters the poor friendly tiger                                  
  """
  squares=[1, 4, 9, 16, 25]
  slsq=squares[2:4]
  return slsq  

print ("annotation : ", func1.__doc__)
ans=func1()
print("< " + "".join([str(ss) + " " for ss in ans]) + ">")

# print (sys.platform)
print (sys.path)
print (sys.builtin_module_names)
print (type(ans))

