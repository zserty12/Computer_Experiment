cc=g++
target = Array

objects = main.o array.o rangearray.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

$(objects) : Array.h RangeArray.h

.PHONY : clean
clean :
	rm $(target) $(objects)
