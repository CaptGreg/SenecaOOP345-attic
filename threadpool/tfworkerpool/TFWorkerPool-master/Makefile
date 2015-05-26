CC=g++
CFLAGS=-c -Wall -std=c++11 -O3 -Wno-unused-result
LDFLAGS=-pthread
SOURCES= main.cpp \
	TFWorkerDispatch.cpp \

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=TFWorkerPool

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXECUTABLE) $(OBJECTS)
