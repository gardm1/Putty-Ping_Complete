# Compiler and linker settings
CC=cl
CFLAGS=/LD /I"lib\libgcrypt\include" /nologo
LDFLAGS=lib\libgcrypt\lib-libgcrypt-1.11.0-ucrt64-gcc\libgcrypt-20.lib \
        lib\libgcrypt\lib-libgcrypt-1.11.0-ucrt64-gcc\libgpg-error-0.lib

# Output DLL name
TARGET=libgcrypt_crypto.dll

# Source files
SRCS=src\libgcrypt_crypto.c \
     src\util\encryption.c \
     src\util\hmac.c \

# Object files (corresponding to source files)
OBJS=$(SRCS:.c=.obj)

# Default rule
all: $(TARGET)

#	lib /out:libgcrypt_crypto.lib $(OBJS)
# Rule to build the DLL
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) /link $(LDFLAGS) /Fe$@
	lib /out:libgcrypt_crypto.lib $(OBJS)

# Rule to build object files
.c.obj:
	$(CC) $(CFLAGS) /c $< /Fo$@

# Clean rule
clean:
	del /q $(OBJS) $(TARGET)
