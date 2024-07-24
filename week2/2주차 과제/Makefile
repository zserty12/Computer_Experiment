cc=gcc
cflags = -W -g
target = Starwars

objects = starwars_main.o starwars_count.o starwars_print.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

$(objects) : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
