  1 #!/usr/bin/env python
  2 import sys, re, operator, string
  3
  4 #
  5 # The functions
  6 #
  7 def read_file(path_to_file):
  8 """
  9 Takes a path to a file and returns the entire
 10 contents of the file as a string
 11 """
 12 with open(path_to_file) as f:
 13     data = f.read()
 14 return data
 15
 16 def filter_chars_and_normalize(str_data):
 17 """
 18 Takes a string and returns a copy with all nonalphanumeric
 19 chars replaced by white space
 20 """
 21 pattern = re.compile('[\W_]+')
 22 return pattern.sub(' ', str_data).lower()
 23
 24 def scan(str_data):
 25 """
 26 Takes a string and scans for words, returning
 27 a list of words.
 28 """
 29 return str_data.split()
 30
 31 def remove_stop_words(word_list):
 32 """
 33 Takes a list of words and returns a copy with all stop
 34 words removed
 35 """
 36 with open('../stop_words.txt') as f:
 37     stop_words = f.read().split(',')
 38 # add single-letter words
 39 stop_words.extend(list(string.ascii_lowercase))
 40 return [w for w in word_list if not w in stop_words]
 41
 42 def frequencies(word_list):
 43 """
 44 Takes a list of words and returns a dictionary associating
 45 words with frequencies of occurrence
 46 """
 47 word_freqs = {}
 48 for w in word_list:
 49     if w in word_freqs:
 50     word_freqs[w] += 1
 51     else:
 52     word_freqs[w] = 1
 53 return word_freqs
 54
 55 def sort(word_freq):
 56 """
 57 Takes a dictionary of words and their frequencies
 58 and returns a list of pairs where the entries are
 59 sorted by frequency
 60 """
 61 return sorted(word_freq.iteritems(), key=operator.itemgetter
   (1), reverse=True)
 62
 63 def print_all(word_freqs):
 64 """
 65 Takes a list of pairs where the entries are sorted by
   frequency and print them recursively.
 66 """
 67 if(len(word_freqs)> 0):
 68    print word_freqs[0][0], ' - ', word_freqs[0][1]
 69    print_all(word_freqs[1:]);
 70
 71 #
 72 # The main function
 73 #
 74 print_all(sort(frequencies(remove_stop_words(scan(
  filter_chars_and_normalize(read_file(sys.argv[1]))))))[0:25])
