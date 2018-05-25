#!/usr/bin/env python
import subprocess

executables = ['twentynine.cpp']
compiler = 'g++'
output_name = ['twentynine']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11' , '-O3', '-pthread','-o{}'.format(out) , exe])