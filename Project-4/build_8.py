#!/usr/bin/env python
import subprocess

executables = ['eight.cpp']
compiler = 'g++'
output_name = ['eight']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11','-O3','-o{}'.format(out) , exe])