CC = g++
LIBS = ../../lib/libode.a ../../lib/libdrawstuff.a -L/usr/X11R6/lib -L/usr/X11/lib -L/usr/lib/X11R6 -L/usr/lib/X11 -lX11 -lGL -lGLU -lm
INCLUDE = -I../../include
PARAMS = -Wall -fno-rtti -fno-exceptions -Wall -fomit-frame-pointer -ffast-math
PARAMS2 = -DdNODEBUG -O2 

test_buggy : test_buggy.o
	$(CC) -o $@ $@.o $(LIBS)

test_buggy.o : test_buggy.cpp
	$(CC) -c $(PARAMS) $(INCLUDE) $(PARAMS) -o test_buggy.o test_buggy.cpp
