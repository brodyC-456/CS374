CC=gcc
CCOPTS=-Wall -Wextra -Werror -pthread
LIBS=

TARGET=grayscaler
SRCS=grayscaler.c ppm.c
OBJS=$(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCOPTS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET) *.o