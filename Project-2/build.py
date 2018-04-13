#!/usr/bin/env python
import subprocess

executables = ['five.java']
compiler = 'javac'

for exe in executables:
	subprocess.call([compiler, exe])