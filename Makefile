CC = g++
CFLAGS = -std=c++11 -Wall -Werror
TARGET = pairing
SOURCES = pairing.cpp

make: $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)
	@echo "Compilation complete."

clean: 
	rm -f $(TARGET)
	@echo "Cleanup complete."