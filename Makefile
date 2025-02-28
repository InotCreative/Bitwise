CC = gcc
CFLAGS = -g
TARGET = ion
SRC = ion.c
VERSION = -std=c18

$(TARGET): $(SRC)
	$(CC) $(VERSION) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)