CC = g++
CFLAGS =  -g -Wall
TARGET = hinfosvc

all: $(TARGET)

$(TARGET): myserver.cpp
	  $(CC) $(CFLAGS) -o $(TARGET) myserver.cpp

clean:
	$(RM) $(TARGET)
