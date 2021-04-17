OBJS=test_log_stream.o log_stream.o 
CFLAGS+=-pthread

target:$(OBJS)
	g++ $^ $(CFLAGS) -o $@

%.o:%.c
	g++ $^ $(CFLAGS) -c -Wall -g -o $@

clean:
	rm *.o target -rf