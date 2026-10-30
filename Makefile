CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = stego
SOURCES = test_encode.c encode.c decode.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) a.out

.PHONY: all clean
