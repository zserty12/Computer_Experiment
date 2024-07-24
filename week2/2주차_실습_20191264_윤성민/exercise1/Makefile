cc=gcc
cflags = -W -g
target = animal
objects = main.o dog.o blackcow.o turtle.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

$(objects) : animal.h

.PHONY : clean
clean :
	rm $(target) $(objects)
