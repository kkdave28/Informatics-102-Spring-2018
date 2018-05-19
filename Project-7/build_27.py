#!/usr/bin/env python
import subprocess

executables = ['twentyseven.cpp']
compiler = 'g++'
output_name = ['twentyseven']

for exe, out in zip(executables, output_name):
	subprocess.call([compiler, '-std=c++11','-O3','-o{}'.format(out) , exe])