INCLUDES=-I/usr/local/include/
CFLAGS=-fno-stack-protector 
#LDFLAGS=-ldl -lpthread -lzmq
LDFLAGS=-lzmq
LIBS=

all: clean compile

compile:
	#gcc client.c -o client $(INCLUDES) $(LDFLAGS) $(LIBS)
	gcc client.c -o client $(INCLUDES) $(LDFLAGS) $(LIBS)

clean:
	rm -f client
