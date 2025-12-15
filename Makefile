CC = gcc
CFLAGS = -Wall -Wextra -g
# This line is critical - it tells Make to build ALL 4 files
OBJ = main.o parser.o execute.o utils.o
TARGET = myshell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c myshell.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET) myshell.log
