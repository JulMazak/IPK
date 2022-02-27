CC = g++
CFLAGS =  -g -Wall -Wextra -Werror
TARGET = hinfosvc

all: $(TARGET)

$(TARGET): myserver.cpp
	  $(CC) $(CFLAGS) -o $(TARGET) myserver.cpp

clean:
	$(RM) $(TARGET)
