# set envrionment
CC=gcc
CC_FLAGS=-D_FILELINE
SRCS=$(wildcard *.c */*.c)

#set inc and lib
INCS=-I .
LIBS=-lpthread -L.

TARGET=rx_demo
image_dir=./data/image
OBJS=$(patsubst %.c, %.o, $(SRCS))
$(shell mkdir -p ${image_dir})

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o:%.c
	$(CC) $(CC_FLAGS) $(INCS) $(LIBS) -c -g $< -o $@

.PHONY: install clean
install:

clean:
	rm $(TARGET) $(OBJS)

