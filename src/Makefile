CC=gcc
TARGET=xtrace

$(TARGET): xtrace.c
	$(CC) -c xtrace.c
	$(CC) xtrace.o -o $(TARGET)

all: $(TARGET)

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) xtrace
