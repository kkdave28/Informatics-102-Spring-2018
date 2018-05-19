#!/usr/bin/env python

import subprocess
import os

executables = ['twentyseven']
interpreter = ''
text_file = 'pride-and-prejudice.txt'
solution_file = 'pride-and-prejudice-solution.txt'


working_dir = os.getcwd()
test_files_location = os.path.join(working_dir, os.pardir)

text_loc = os.path.join(test_files_location, text_file)
solution_loc = os.path.join(test_files_location, solution_file)

output = None
for exe in executables:

	print('--------{}-----------'.format(exe))
	file_path = os.path.join(working_dir, exe)

	if interpreter == '':
		output = subprocess.check_output([file_path,text_loc]).splitlines()
	else:
		output = subprocess.check_output([interpreter, file_path, text_loc]).splitlines()

	solution = ""
	with open (solution_loc, "r") as f:
	    solution = f.read().splitlines()

passing = True
number_of_line = 0
for out, sol in zip(output[-25:], solution):
    number_of_line+=1

    if out == sol:
        continue
    else:
        print "Expected: {}, \tActually: {}".format(sol, out)
        passing = False

if passing and number_of_line == 25:
    print "Passed!"
else:
    print "Failed..."