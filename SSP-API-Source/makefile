
LIBTARG = libsspapi.so

# For http interface (openssl or mbedtls)
EX1TARG = ssphttp
# For function interface
EX2TARG = sspfunc
# For simple 'ping' function
EX3TARG = ssptest
# For security memory tests
EX4TARG = test_secure_memory

LIBOBJS = base64url.o browser.o client.o configdata.o crypto.o database.o dbglog.o global.o handler.o handler-mbedtls.o handler-openssl.o pendingauths.o qrcode.o response.o server.o sspapi.o utils.o

EX1OBJS = ssphttp.o
EX2OBJS = sspfunc.o
EX3OBJS = ssptest.o
EX4OBJS = test_secure_memory.o

LIBLIBS = -lblowfish -ldb -llodepng -lqrencode -lsodium -lssl -lcrypto -lmbed -pthread
EX1LIBS = -lsspapi
EX2LIBS = -lsspapi
EX3LIBS = -lsspapi
EX4LIBS = -lpthread

#GCC = gcc
#GCC = gcc -g
#GCC = gcc -D DEBUG_LOG
GCC = gcc -g -D DEBUG_LOG

# Not to be confused with filenames
.PHONY: default all clean test_secure_memory test

# Not to be deleted
.PRECIOUS:

default: $(LIBTARG) $(EX1TARG) $(EX2TARG) $(EX3TARG)

all: default

clean:
	-rm -f *.o
	-rm -f $(LIBTARG) $(EX1TARG) $(EX2TARG) $(EX3TARG) $(EX4TARG)

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)

# Make object files $@=target, $<=first prerequisite
%.o: %.c $(HEADERS)
	@echo Sources:
	@echo $(SOURCES)
	@echo Headers:
	@echo $(HEADERS)
	@echo

	$(GCC) -c -Wall -Werror -fPIC -o $@ $<

ssphttp.o: sspmain.c $(HEADERS)
	$(GCC) -c -Wall -Werror -fPIC -o ssphttp.o sspmain.c -D HTTP_INTERFACE
sspfunc.o: sspmain.c $(HEADERS)
	$(GCC) -c -Wall -Werror -fPIC -o sspfunc.o sspmain.c -D FUNC_INTERFACE
ssptest.o: sspmain.c $(HEADERS)
	$(GCC) -c -Wall -Werror -fPIC -o ssptest.o sspmain.c -D TEST_INTERFACE
test_secure_memory.o: test_secure_memory.c $(HEADERS)
	$(GCC) -c -Wall -Werror -fPIC -o test_secure_memory.o test_secure_memory.c

# Make shared library
libsspapi.so: $(LIBOBJS)
	gcc -shared -o $(LIBTARG) $(LIBOBJS) $(LIBLIBS)
	sudo install -m 755 libsspapi.so /usr/local/lib
	sudo ldconfig
	
# Make executables
ssphttp: $(EX1OBJS) $(LIBOBJS)
	$(GCC) -Wall -Werror -o $(EX1TARG) $(EX1OBJS) $(EX1LIBS)
	#cp ssphttp ~/Desktop/SSP-API-Demo
sspfunc: $(EX2OBJS) $(LIBOBJS)
	$(GCC) -Wall -Werror -o $(EX2TARG) $(EX2OBJS) $(EX2LIBS)
ssptest: $(EX3OBJS) $(LIBOBJS)
	$(GCC) -Wall -Werror -o $(EX3TARG) $(EX3OBJS) $(EX3LIBS)
test_secure_memory: $(EX4OBJS) utils.o
	$(GCC) -Wall -Werror -o $(EX4TARG) $(EX4OBJS) utils.o $(EX4LIBS)

# Run security tests
test: test_secure_memory
	@echo "Running security memory tests..."
	./$(EX4TARG)
