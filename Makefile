# Makefile for Octave extension matpow

LIBS=-lmpg123

.phony: all
all: mp3close.oct  mp3format.oct  mp3init.oct  mp3open.oct  mp3read.oct


.phony: clean
clean:
	rm -f *.oct *.o


.phony: distclean


distclean: clean
	rm -f *~ octave-core core

#.phony: test
#test: matpow.oct
#        octave --silent test_matpow.m

%.oct: %.cc
	mkoctfile $(LIBS) $<
    
