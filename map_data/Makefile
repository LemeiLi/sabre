# set envrionment
CC=aarch64-linux-gnu-gcc
CC_FLAGS=-D_FILELINE
SRCS=$(wildcard *.c */*.c */*/*.c)

#set inc and lib
INCS=-I .
INCS+=-I ./include/
LIBS=-lpthread -L.

TARGET=test_flc
OBJS=$(patsubst %.c, %.o, $(SRCS))

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o:%.c
	$(CC) $(CC_FLAGS) $(INCS) $(LIBS) -c -g $< -o $@

.PHONY: install clean
install:

clean:
	rm $(TARGET) $(OBJS)

