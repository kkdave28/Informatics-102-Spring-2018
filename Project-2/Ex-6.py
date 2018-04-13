 1 #!/usr/bin/env python
 2 import re, sys, collections
 3
 4 stops = open('../stop_words.txt').read().split(',')
 5 words = re.findall('[a-z]{2,}', open(sys.argv[1]).read().lower())
 6 counts = collections.Counter(w for w in words if w not in stops)
 7 for (w, c) in counts.most_common(25):
 8 print w, '-', c
