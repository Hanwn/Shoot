OBJS=main.o task_queue.o thread_pool.o
CFLAGS+=-pthread

thread_pool:$(OBJS)
	g++ $^ $(CFLAGS) -o $@


%.o:%.c
	g++ $^ $(CFLAGS) -c -o $@

clean:
	rm *.o thread_pool -rf