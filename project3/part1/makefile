CC = gcc
CFLAGS = -g

OBJS = indexPage.o crawler.o

all: webSearch
webSearch : webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@ -g

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o *.txt