# python version of c++ example from python doc http://www.aleax.it/Python/accu03_Python_Intro_alex.pdf

# build a word -> line numbers mapping
idx = {}
for n,line in enumerate (open('words.cpp')):
  for word in line.split():
    idx.setdefault(word,[]).append(n+1)
# display by alphabetically-sorted word
words = idx.keys()
words.sort()
for word in words:
  print "%s -->" % word,
  for n in idx[word]: print n,
  print
