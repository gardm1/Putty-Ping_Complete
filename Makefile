CC = cl
LINK = link

TARGET = toolbox.dll
IMPLIB = toolbox.lib

SOURCES = src\libgcrypt_crypto.c src\process.c src\util\encryption.c src\util\hmac.c
OBJECTS = libgcrypt_crypto.obj process.obj encryption.obj hmac.obj

INCLUDES = -Ilib\libgcrypt\include -Isrc -Isrc\util

LIBDIRS = -LIBPATH:lib\libgcrypt\lib-libgcrypt-1.11.0-ucrt64-gcc
LIBS = libgcrypt-20.lib libgpg-error-0.lib iphlpapi.lib ws2_32.lib

CFLAGS = /nologo /c /O2 /W3 /D MYDLL_EXPORTS $(INCLUDES)

LFLAGS = /nologo /DLL

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) $(LFLAGS) /OUT:$(TARGET) $(OBJECTS) $(LIBDIRS) $(LIBS)

libgcrypt_crypto.obj: src\libgcrypt_crypto.c
	$(CC) $(CFLAGS) src\libgcrypt_crypto.c

process.obj: src\process.c
	$(CC) $(CFLAGS) src\process.c

encryption.obj: src\util\encryption.c
	$(CC) $(CFLAGS) src\util\encryption.c

hmac.obj: src\util\hmac.c
	$(CC) $(CFLAGS) src\util\hmac.c

clean:
	del src\*.obj src\util\*.obj bin\$(TARGET) src\$(IMPLIB) src\*.exp

.PHONY: all clean

