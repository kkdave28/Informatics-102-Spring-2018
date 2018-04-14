#!/usr/bin/env python
import subprocess

executables = ['five_1.java']
compiler = 'javac'

for exe in executables:
	subprocess.call([compiler, exe])