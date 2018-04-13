#!/usr/bin/env python

import subprocess
import os

executables = ['five']
interpreter = 'java'
text_file = 'pride-and-prejudice.txt'
stop_words_file = 'stop_words.txt'
solution_file = 'pride-and-prejudice-solution.txt'


working_dir = os.getcwd()
test_files_location = os.path.join(working_dir, os.pardir)

text_loc = os.path.join(test_files_location, text_file)
stop_words_loc = os.path.join(test_files_location, stop_words_file)
solution_loc = os.path.join(test_files_location, solution_file)

output = None
for exe in executables:
	print('--------{}-----------'.format(exe))
	file_path = os.path.join(working_dir, exe)

	if interpreter == '':
		output = subprocess.check_output([file_path, stop_words_loc, text_loc]).splitlines()
	else:
		file_path = exe
		output = subprocess.check_output([interpreter, file_path, text_loc, stop_words_loc]).splitlines()

	solution = ""
	with open (solution_loc, "r") as f:
	    solution = f.read().splitlines()

	passing = True
	for out, sol in zip(output, solution):

		if out == sol:
			continue
		else:
			print "Expected: {}, \tActually: {}".format(sol, out)
			passing = False

	if passing:
		print "Passed!"
	else:
		print "Failed..."