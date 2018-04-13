 1 #!/usr/bin/env python
  2
  3 import sys, string
  4 # the global list of [word, frequency] pairs
  5 word_freqs = []
  6 # the list of stop words
  7 with open('../stop_words.txt') as f:
  8 stop_words = f.read().split(',')
  9 stop_words.extend(list(string.ascii_lowercase))
 10
 11 # iterate through the file one line at a time
 12 for line in open(sys.argv[1]):
 13 start_char = None
 14 i = 0
 15 for c in line:
 16    if start_char == None:
 17        if c.isalnum():
 18        # We found the start of a word
 19        start_char = i
 20    else:
 21        if not c.isalnum():
 22        # We found the end of a word. Process it
 23        found = False
 24        word = line[start_char:i].lower()
 25        # Ignore stop words
 26        if word not in stop_words:
 27         pair_index = 0
 28         # Let's see if it already exists
 29         for pair in word_freqs:
 30         if word == pair[0]:
 31            pair[1] += 1
 32            found = True
 33            found_at = pair_index
 34            break
 35         pair_index += 1
 36         if not found:
 37         word_freqs.append([word, 1])
 38         elif len(word_freqs)> 1:
 39         # We may need to reorder
 40         for n in reversed(range(pair_index)):
 41            if word_freqs[pair_index][1]>
              word_freqs[n][1]:
 42                # swap
 43                word_freqs[n], word_freqs[
               pair_index] = word_freqs[
               pair_index], word_freqs[n]
 44                pair_index = n
 45        # Let's reset
 46        start_char = None
 47    i += 1
 48
 49 for tf in word_freqs[0:25]:
 50    print tf[0], ' - ', tf[1]
