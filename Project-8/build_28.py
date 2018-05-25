#!/usr/bin/env python
import subprocess

executables = ['twentyeight.cpp']
compiler = 'g++'
output_name = ['twentyeight']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11' , '-O3', '-pthread','-o{}'.format(out) , exe])