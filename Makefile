
#LDLIBS = -L/usr/lib -I/usr/include
#INCL = -I/usr/include/c++/4.3

CC = gcc
CPP = gcc

EXEC = fdiconv
OBJS = fdiconv.o

CPPFLAGS += -Wall -std=c99
#CPPFLAGS += -ggdb

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

%.o: %.cpp
	$(CPP) -c $(CPPFLAGS) -o $@ $< $(INCL)
