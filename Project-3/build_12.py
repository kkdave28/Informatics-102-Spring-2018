#!/usr/bin/env python
import subprocess

executables = ['twelveone.cpp']
compiler = 'g++'
output_name = ['twelveone']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11', '-o{}'.format(out) , exe])