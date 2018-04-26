#!/usr/bin/env python
import subprocess

executables = ['nine_one.cpp']
compiler = 'g++'
output_name = ['nineone']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11','-O3','-o{}'.format(out) , exe])