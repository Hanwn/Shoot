OBJS=test_stdio.o file2disk.o
CFLAGS+=-pthread

target:$(OBJS)
	g++ $^ $(CFLAGS) -o $@

%.o:%.c
	g++ $^ $(CFLAGS) -c -Wall -g -o $@

clean:
	rm *.o target -rf