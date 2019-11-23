sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -shared -g -lm -lcurl -ldl -fPIC -rdynamic


.PHONY: all
all: libhermes_curl.so

libhermes_curl.so: $(objects)
	gcc $(flags) -o $@ $^

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

#install:
#	make
#	make libhermes_curl.so

clean:
	-rm *.out
	-rm *.o
	-rm *.so
	-rm *.a
	-rm src/*.o
