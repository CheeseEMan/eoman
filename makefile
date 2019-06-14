TARGET=eoman
CC=gcc
MAIN=main.c
CFLAGS=-Wall `pkg-config --cflags gtk+-3.0`

#Libraries
SYS_LIBS=`pkg-config --libs gtk+-3.0`
LOC_LIBS= src/get-package.c

all: $(TARGET)

$(TARGET): $(MAIN)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN) $(LOC_LIBS) $(SYS_LIBS)
