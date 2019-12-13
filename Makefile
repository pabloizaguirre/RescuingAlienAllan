CC = gcc -pedantic
CFLAGS = -Wall
EXE = main 
#LIBRARIES = enemies.o level.o map.o people.o print_on_screen.o
#LIBRARIES =print_on_screen.o read_from_file.o
LIBRARIES =print_on_screen.o read_from_file.o level.o read_keys.o map.o people.o design.o


all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o $(LIBRARIES)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIBRARIES)

enemies.o: enemies.c enemies.h
	$(CC) $(CFLAGS) -c $<

read_keys.o: read_keys.c read_keys.h
	$(CC) $(CFLAGS) -c $<

level.o: level.c level.h
	$(CC) $(CFLAGS) -c $<

map.o: map.c map.h
	$(CC) $(CFLAGS) -c $<

people.o: people.c people.h
	$(CC) $(CFLAGS) -c $<

design.o: design.c design.h
	$(CC) $(CFLAGS) -c $<

print_on_screen.o: print_on_screen.c print_on_screen.h
	$(CC) $(CFLAGS) -c $<

read_from_file.o: read_from_file.c read_from_file.h
	$(CC) $(CFLAGS) -c $<

run:
	./test

