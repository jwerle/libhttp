
CC ?= cc
VALGRIND ?= valgrind
OS = $(shell uname)

SRC = $(filter-out src/main.c, $(wildcard src/*.c))
SRC += $(wildcard deps/*/*.c)
OBJS = $(SRC:.c=.o)
TESTS = $(wildcard tests/*.c)

PREFIX ?= /usr/local
MANPREFIX ?= $(PREFIX)/share/man

MAN_FILES = $(wildcard man/*.md)

BIN ?= http
LIB_NAME ?= http

VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_PATCH = 3

TARGET_NAME = lib$(LIB_NAME)
TARGET_STATIC = $(TARGET_NAME).a
TARGET_DSOLIB = $(TARGET_NAME).so.$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
TARGET_DSO = $(TARGET_NAME).so

CFLAGS += -std=c99 -Wall -O2 -fvisibility=hidden -fPIC
CFLAGS += -DHTTP_VERSION='"$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)"'
CFLAGS += -Ideps -Iinclude -Iinclude/http -DHTTP_LIB

LDFLAGS += -o $(TARGET_DSOLIB) -shared -soname $(TARGET_DSO).$(VERSION_MAJOR)

ifeq ($(OS), Darwin)
	LDFLAGS += -lc -Wl,-install_name,$(TARGET_DSO)
endif

$(BIN): $(TARGET_STATIC) $(TARGET_DSO)
	@#$(CC) $(CFLAGS) $(SRC) src/main.c -o $(BIN)

install: $(BIN)
	mkdir -p $(PREFIX)/include/$(LIB_NAME)
	install include/http.h $(PREFIX)/include
	cp include/http/*.h $(PREFIX)/include/http
	install $(TARGET_DSO) $(PREFIX)/lib

$(TARGET_STATIC): $(OBJS)
	ar crus $(TARGET_STATIC) $(OBJS)

$(TARGET_DSO): $(OBJS)
ifeq ("Darwin","$(OS)")
	$(CC) -shared $(OBJS) $(OSX_LDFLAGS) -o $(TARGET_DSOLIB)
	ln -s $(TARGET_DSOLIB) $(TARGET_DSO)
	ln -s $(TARGET_DSOLIB) $(TARGET_DSO).$(VERSION_MAJOR)
else
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET_DSOLIB)
	ln -s $(TARGET_DSOLIB) $(TARGET_DSO)
	ln -s $(TARGET_DSOLIB) $(TARGET_DSO).$(VERSION_MAJOR)
	strip --strip-unneeded $(TARGET_DSO)
endif

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

test: $(OBJS) $(TARGET_STATIC) deps $(TESTS)

$(TESTS):
	@cc $(@) deps/ok/libok.a -L. -lhttp $(CFLAGS) -o $(@:.c=)
	@./$(@:.c=)

deps:
	make -C deps/ok

clean:
	make clean -C deps/ok
	@#make clean -C examples
	rm -f $(OBJS)
	rm -f $(BIN)
	rm -f $(TARGET_STATIC)
	rm -f $(TARGET_DSO)
	rm -f $(TARGET_DSOLIB)
	rm -f *.so*
	$(foreach test, $(TESTS), $(shell rm -f $(test:.c=)))

docs: $(MAN_FILES)

examples: $(TARGET_DSO)
	make -C examples

$(MAN_FILES):
	curl -# -F page=@$(@) -o $(@:%.md=%) http://mantastic.herokuapp.com

.PHONY: deps $(MAN_FILES) $(TESTS) examples
