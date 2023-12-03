LINUXLIBS  = -lm -lGL -lGLU -lglut
MACLIBS = -lm -framework OpenGL -framework GLUT
CFLAGS = -Wall

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)

linux: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LINUXLIBS)

mac: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(MACLIBS)