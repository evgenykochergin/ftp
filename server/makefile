CC := gcc
CFLAGS := -Wall -g -Os

SHDIR := ../common

OBJS = ftpserver.o $(SHDIR)/common.o

all: ftpserver

ftserve: $(OBJS)
	@$(CC) -o ftpserver $(CFLAGS) $(OBJS)

$(OBJS) : %.o: %.c 
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY:
clean:
	@rm -f *.o ftpserver
	@rm -f ../common/*.o
	@echo Done cleaning
