OBJS=epoll.o http.o main.o Server.o task_queue.o thread_pool.o utils.o time.o heap.o 
CFLAGS+=-pthread

target:$(OBJS)
	g++ $^ $(CFLAGS) -o $@

%.o:%.c
	g++ $^ $(CFLAGS) -c -Wall -g -o $@

clean:
	rm *.o target -rf