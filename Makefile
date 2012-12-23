CC := g++
CFLAGS := -lssl
TARGET := smtp
OBJECTS := main.o base64.o smtpclient.o 

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(TARGET)

main.o: main.cpp
	$(CC) -c main.cpp
base64.o: base64.cpp
	$(CC) -c base64.cpp
smtpclient.o: smtpclient.cpp
	$(CC) -c smtpclient.cpp
clean:
	rm -f $(OBJECTS)
