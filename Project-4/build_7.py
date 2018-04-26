#!/usr/bin/env python
import subprocess

executables = ['seven.cpp']
compiler = 'g++'
output_name = ['seven']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11','-O3','-o{}'.format(out) , exe])