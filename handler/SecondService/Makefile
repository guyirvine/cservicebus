INCLUDES=-I/usr/include/apr-1.0 -I/usr/local/cservicebus/include
CFLAGS=-fno-stack-protector 
LDFLAGS=-D_LARGEFILE64_SOURCE -shared
BASE_DIR=/usr/local/cservicebus
LIBS=/usr/lib/libapr-1.a

all:	compile install

compile:
	gcc -fPIC -c libmhsecondservice.c $(LDFLAGS) $(INCLUDES) $(CFLAGS)
	ld -shared -soname libmhsecondservice.so.1 -o libmhsecondservice.so.1.0 -lc libmhsecondservice.o $(LIBS)

install:
	cp libmhsecondservice.so.1.0 $(BASE_DIR)/lib/message_handler/libmhsecondservice.so.1.0

clean:
	rm -f *.o *.so* test_lib
