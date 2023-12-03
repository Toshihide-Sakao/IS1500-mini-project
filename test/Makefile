LIBS  = -lm
CFLAGS = -Wall

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)

linux: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)