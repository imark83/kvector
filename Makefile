CC = g++

COMMON_OBJECTS = obj/main.o obj/data.o
DEBUG_COMMON_OBJECTS = obj/debug/main.o obj/debug/data.o 
ALL_OBJECTS = $(DEBUG_COMMON_OBJECTS) $(COMMON_OBJECTS)


LIBS = -lopencv_imgcodecs -lopencv_core -lopencv_highgui -lopencv_imgproc
CFLAGS = -O3 -Wall -I /usr/local/include/opencv4 
DEBUGFLAGS = -O0 -g -I /usr/local/include/opencv4 

database: $(COMMON_OBJECTS) obj/kdtree.o
	$(CC) -o $@ $(COMMON_OBJECTS) obj/kdtree.o $(LIBS)

debug: clean $(DEBUG_COMMON_OBJECTS)
	$(CC) -o $@ $(DEBUG_COMMON_OBJECTS) $(LIBS)

obj/kdtree.o: kdtree/kdtree.c
	gcc -c kdtree/kdtree.c -o kdtree/kdtree.o -O3 -Wall

obj/debug/%.o: %.cpp
	$(CC) -c -o $@ $< $(DEBUGFLAGS)

obj/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)


all: clean
	make database
	make debug


.PHONY: clean
clean:
	rm -f $(ALL_OBJECTS) database debug
