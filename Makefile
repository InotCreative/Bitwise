CC = gcc
CFLAGS = -g
TARGET = ion
SRC = ion.c


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)