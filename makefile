CC = g++

GCCFLAGS = -O

INCLUDE = -I. -I/usr/include 



FILENAME = main

LDLIBS =   -lglut -lXext -lX11 -lXmu -lXi -lGL -lGLU -lm -lSDL 
LDFLAGS =   -L/usr/X11R6/lib -L/usr/lib -L/lib

program:
	$(CC) $(GCCFLAGS) $(INCLUDE) $(FILENAME).cpp event.cpp objectdisplay.cpp object.cpp objectmanager.cpp vector2d.cpp rect.cpp polygon.cpp circle.cpp bezier.cpp buttonmenu.cpp imageloader.cpp $(LDFLAGS) $(LDLIBS) -o build/ExeSketch