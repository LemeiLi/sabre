SOURCE = $(wildcard ./test/*.c)
TARGETS = $(patsubst %.c, %, $(SOURCE))

CC = gcc
CFLAGS = -Wall -g -I./inc
LDFLAGS = -L./lib -lhal -lpthread -lpcap

all:$(TARGETS)

$(TARGETS):%:%.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

.PHONY:clean docs
clean:
	-rm -rf $(TARGETS)
docs:
	-doxygen Doxyfile.release
	firefox ./docs/html/index.html &




