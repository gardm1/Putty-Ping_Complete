CC=cl
CFLAGS=/LD /I"lib\libgcrypt\include" /nologo
LDFLAGS=/link lib\libgcrypt\lib-libgcrypt-1.11.0-ucrt64-gcc\libgcrypt-20.lib \
        lib\libgcrypt\lib-libgcrypt-1.11.0-ucrt64-gcc\libgpg-error-0.lib

TARGET=libgcrypt_crypto.dll

SRCS=src\libgcrypt_crypto.c \
     src\util\encryption.c \
     src\util\hmac.c \

OBJS=$(SRCS:.c=.obj)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) /Fe$@
	lib /out:libgcrypt_crypto.lib $(OBJS)
	cl /c src\process.c /link ws2_32.lib iphlpapi.lib
	lib /out:process.lib process.obj
	cl $(CFLAGS) src\toolbox.c libgcrypt_crypto.lib process.lib $(LDFLAGS)

.c.obj:
	$(CC) $(CFLAGS) /c $< /Fo$@

clean:
	del src\*.obj src\*.dll src\*.lib src\*.exp bin\toolbox.dll
