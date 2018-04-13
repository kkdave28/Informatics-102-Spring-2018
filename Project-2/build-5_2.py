#!/usr/bin/env python
import subprocess

executables = ['five_2.java']
compiler = 'javac'

for exe in executables:
	subprocess.call([compiler, exe])