CC = g++
INCDIR = .
LIBDIR = .
CFLAGS = -Wno-write-strings -pg -O3 -fopenmp

SOURCES = \
	ane.cpp \
	application.cpp \
	filtering.cpp \
	maxtree.cpp \
	mm.cpp \
	recognition.cpp \
	segmentation.cpp


OBJECTS = $(SOURCES:.cpp=.o)



##### REGRAS #####

.cpp.o:
	$(CC) -c -I$(INCDIR) $(CFLAGS) $(GLUT_CFLAGS) $<



##### ACOES #####

default: max

clean:
	-rm -f *.o
	-rm -f *~
	-rm maxtree 

max: $(LIBDIR)

# Construindo a biblioteca
$(LIBDIR): $(OBJECTS)
	$(CC) -o maxtree $(OBJECTS) $(CFLAGS)

