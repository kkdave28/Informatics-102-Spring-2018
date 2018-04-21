#!/usr/bin/env python
import subprocess

executables = ['fourteenone.cpp']
compiler = 'g++'
output_name = ['fourteenone']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11', '-o{}'.format(out) , exe])