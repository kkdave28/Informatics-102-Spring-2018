#!/usr/bin/env python
import sys, re, operator, string
from abc import ABCMeta
import atexit
import inspect
#import sixteen
#
# The classes
#
class TFExercise():
    __metaclass__ = ABCMeta

    def info(self,ignore=None):
        return self.__class__.__name__

class DataStorageManager(TFExercise):
    """ Models the contents of the file """
    
    def __init__(self, path_to_file=None):
        with open(path_to_file) as f:
            self._data = f.read()
        pattern = re.compile('[\W_]+')
        self._data = pattern.sub(' ', self._data).lower()

    def words(self):
        """ Returns the list words in storage """
        return self._data.split()

    def info(self):
        print( super(DataStorageManager, self).info() + ": My major data structure is a " + self._data.__class__.__name__ +"\n"+"Methods implemented are:\n" + "- words(self)\n" + "- info(self)\n")

class StopWordManager(TFExercise):
    """ Models the stop word filter """
    
    def __init__(self,ignore=None):
        with open('../stop_words.txt') as f:
            self._stop_words = f.read().split(',')
        # add single-letter words
        self._stop_words.extend(list(string.ascii_lowercase))

    def is_stop_word(self, word):
        return word in self._stop_words

    def info(self):
        print(super(StopWordManager, self).info() + ": My major data structure is a " + self._stop_words.__class__.__name__ + "\n"+"Methods implemented are:\n" + "- is_stop_word(self,word)\n" + "- info(self)\n")

class WordFrequencyManager(TFExercise):
    """ Keeps the word frequency data """
    
    def __init__(self, ignore=None):
        self._word_freqs = {}

    def increment_count(self, word):
        if word in self._word_freqs:
            self._word_freqs[word] += 1
        else:
            self._word_freqs[word] = 1

    def sorted(self):
        return sorted(self._word_freqs.iteritems(), key=operator.itemgetter(1), reverse=True)

    def info(self):
        print( super(WordFrequencyManager, self).info() + ": My major data structure is a " + self._word_freqs.__class__.__name__ +" \n" +"Methods implemented are:\n" + "- increment_count(self,word)\n"+ "- sorted(self)\n" + "- info(self)\n")

class WordFrequencyController(TFExercise):
    def __init__(self, path_to_file=None):
        self._storage_manager = DataStorageManager(path_to_file)
        self._stop_word_manager = StopWordManager()
        self._word_freq_manager = WordFrequencyManager()
        #atexit.register(self._stop_word_manager.info)
        #atexit.register(self._storage_manager.info)
        #atexit.register(self._word_freq_manager.info)
        #atexit.register(TFExercise.info)
        #atexit.register(self.info)

    def run(self):
        for w in self._storage_manager.words():
            if not self._stop_word_manager.is_stop_word(w):
                self._word_freq_manager.increment_count(w)

        word_freqs = self._word_freq_manager.sorted()
        for (w, c) in word_freqs[0:25]:
            print w, ' - ', c
    def info(self):
        print(super(WordFrequencyController,self).info() + ": Classes I manage are " + self._storage_manager.__class__.__name__ + " " + self._stop_word_manager.__class__.__name__+" "+ self._word_freq_manager.__class__.__name__ +"\n"+"Methods implemented are:\n" + "- run(self)\n"+ "- info(self)\n")

#
# The main function
#
if __name__ == "__main__":
    WordFrequencyController(sys.argv[1]).run()
    print("\n")
    import sixteen
    for name, data in inspect.getmembers(sixteen, inspect.isclass):
        if name == '__builtins__':
            continue
        if name != 'ABCMeta' and name != "TFExercise":
            str = name +"(sys.argv[1]).info()"
            #print(str)
            eval(str)
        #print '%s :' % name, repr(data)
    