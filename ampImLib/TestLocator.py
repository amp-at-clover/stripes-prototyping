#!/usr/bin/env python
from os import listdir, system
import sys
from re import match

if len( sys.argv ) < 2 or len( sys.argv ) > 3:
	print "Usage: %s [ directory where files are located  ] [ gabLocalize/fastLocalize ] \n"%(sys.argv[0])
	sys.exit( -1 )

dirList = listdir( '%s'%(sys.argv[1]) )

localizer = 'fastLocalize'
if len( sys.argv )==3 and str(sys.argv[2])=='gabLocalize':
	localizer = 'gabLocalize'

for file in dirList:
	if match( '[\w]*.[jpg|JPG|jp2|JP2]{3}$',file):
		#runStr = './gabLocalize %s/%s'%(sys.argv[1],file)
		runStr = './%s %s/%s'%(localizer, sys.argv[1],file)
		print runStr
		system( runStr )
