INCLUDES=-I/usr/local/apr/include/apr-1
CFLAGS=-fno-stack-protector 
LDFLAGS=-ldl -lpthread -lzmq
LIBS=/usr/local/apr/lib/libaprutil-1.a /usr/local/apr/lib/libapr-1.a

all: clean compile install

compile:
	gcc main.c -o cservicebus $(INCLUDES) $(LDFLAGS) $(LIBS)

install:
	cp libhandler.h /usr/local/cservicebus/include/
	cp libhandlermgr.h /usr/local/cservicebus/include/

clean:
	rm -f cservicebus
