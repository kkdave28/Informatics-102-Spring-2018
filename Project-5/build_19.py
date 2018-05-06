#!/usr/bin/env python
import subprocess
compiler = 'g++'
subprocess.call([compiler,'-std=c++11', '-fPIC', '-shared', 'words1.cpp', '-owords1.so'])
subprocess.call([compiler,'-std=c++11', '-fPIC', '-shared', 'words2.cpp', '-owords2.so'])
subprocess.call([compiler,'-std=c++11', '-fPIC', '-shared', 'freq1.cpp', '-ofreq1.so'])
subprocess.call([compiler,'-std=c++11', '-fPIC', '-shared', 'freq2.cpp', '-ofreq2.so'])
subprocess.call([compiler,'nineteen.cpp','words1.so','words2.so', 'freq1.so','freq2.so','-std=c++11','-ldl','-rdynamic','-onineteen'])
#g++ nineteen.cpp words1.so words2.so freq1.so freq2.so -std=c++11 -ldl -rdynamic -o nineteen
#