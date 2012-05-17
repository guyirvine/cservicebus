INCLUDES=-I/usr/local/apr/include/apr-1 -I/usr/local/cservicebus/include
CFLAGS=-fno-stack-protector 
#LDFLAGS=-D_LARGEFILE64_SOURCE -shared
LDFLAGS=-dynamiclib
BASE_DIR=/usr/local/cservicebus
LIBS=/usr/local/apr/lib/libapr-1.a

all:	compile install

compile:
#	gcc -fPIC -c libmhaddressservice.c $(LDFLAGS) $(INCLUDES) $(CFLAGS)
	gcc -fPIC -c -o libmhaddressservice.o libmhaddressservice.c $(INCLUDES)
#	ld -shared -soname libmhaddressservice.so.1 -o libmhaddressservice.so.1.0 -lc libmhaddressservice.o $(LIBS)
#	ld libmhaddressservice.o -dynamic -lc -o libmhaddressservice.so.1.0  $(LIBS)
	gcc -dynamiclib -o libmhaddressservice.so.1.0 libmhaddressservice.o

install:
	cp libmhaddressservice.so.1.0 $(BASE_DIR)/lib/message_handler/libmhaddressservice.so.1.0
#	ln -s $(BASE_DIR)/lib/message_handler/libmhaddressservice.so.1.0 $(BASE_DIR)/lib/message_handler/libmhaddressservice.so.1
#	ln -s $(BASE_DIR)/lib/message_handler/libmhaddressservice.so.1 $(BASE_DIR)/lib/message_handler/libmhaddressservice.so

clean:
	rm -f *.o *.so* test_lib
