#!/usr/bin/python3
#  imports
## full imports
import sys, os, glob
## partial imports
from getopt     import getopt, GetoptError
from subprocess import run, Popen
from subprocess import DEVNULL as devnull
from subprocess import STDOUT  as stdout
from subprocess import PIPE    as pipe

scriptname = os.path.basename(__file__)

s1 = r'''https://www.wolframalpha.com/input/?i=N%5B(0.5*(erf((-3+%2B+(6%2F'''
s2 = r''')+*+'''
s3 = r''')%2Fsqrt(2))-erf((-3+%2B+(6%2F'''
s4 = r''')+*+('''
s5 = r'''+-+1))%2Fsqrt(2))))%2F0.997300203936739810946696370464810,+33%5D'''
perlregex = r"""print "$1 $3\n" if /<a ng-href="\/input\/\?i=([0-9]*\.[0-9]*)(%C3%9710%5E((\+|-)?[0-9]+))?/"""
fname = 'res.html'

def myeval(ntotal, ncurrent):
	r1 = None
	r2 = None
	res = None
	err = None
	tt = str(ntotal)
	cc = str(ncurrent)
	print('>> ' + tt + ' ' + cc)
	url = s1 + (tt + s2 + cc) + s3 + (tt + s4 + cc) + s5
	## try run computations
	try:
		## execute computations
		r1 = run(['phantomjs', 'get.js', url], stdout=pipe, stderr=stdout)
		if not r1.returncode == 0:
			raise Exception('err')
		## extract result
		## universal_newlines=True needs to pass 'input' as string, not a byte stream
		r2 = Popen(['perl', '-ne', perlregex], stdout=pipe, stdin=pipe, stderr=stdout, universal_newlines=True)#, stdin=pipe, stderr=stdout)
		res, err = r2.communicate(input=r1.stdout.decode('utf-8'))
	except:
		print('run failed ):')
		sys.exit(1)
	## extract result
	return res

arr = [[21, 3],[21, 6],
[25, 10],[25, 11],
[31, 1],[31, 3],
[31, 7],[43, 19],
[43, 21]]

#for t, i in arr:
#	x = ''
#	while x == '':
#		x = myeval(t, i).strip()
#	with open('res3.csv', 'a') as myfile:
#		myfile.write(str(t)+'00000'+str(i)+',"'+x+'"\n')
#	print(x)

for t in range(2, 51, 1):
	print('Length: ' + str(t))
	for i in range(1, 1 + t // 2):
		x = ''
		while x == '':
			x = myeval(t, i).strip()
		with open('res.csv', 'a') as myfile:
			myfile.write(str(t)+','+str(i)+',"'+x+'"\n')
		print(x)
	print('')
