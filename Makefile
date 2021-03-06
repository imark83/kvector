CC = g++

COMMON_OBJECTS = obj/main.o obj/data.o
DEBUG_COMMON_OBJECTS = obj/debug/main.o obj/debug/data.o

ALL_OBJECTS = $(DEBUG_COMMON_OBJECTS) $(COMMON_OBJECTS)


# LIBS = -lgmpxx -lgmp
CFLAGS = -O3 -Wall
DEBUGFLAGS = -O0 -g

database: $(COMMON_OBJECTS)
	$(CC) -o $@ $(COMMON_OBJECTS) $(LIBS)

debug: clean $(DEBUG_COMMON_OBJECTS)
	$(CC) -o $@ $(DEBUG_COMMON_OBJECTS) $(LIBS)


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
