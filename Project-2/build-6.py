#!/usr/bin/env python
import subprocess

executables = ['six.java']
compiler = 'javac'

for exe in executables:
	subprocess.call([compiler, exe])