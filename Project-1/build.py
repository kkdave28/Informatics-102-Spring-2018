#!/usr/bin/env python
import subprocess

executables = ['frequency.cpp']
compiler = 'g++'
output_name = ['frequency']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11', '-o{}'.format(out) , exe])