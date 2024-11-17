CC = cl
#CFLAGS = /nologo /W3 /D_UNICODE /DUNICODE /I"C:\Users\gardm\usr\include\libgcrypt"
CFLAGS = /nologo /W2 /D_UNICODE /DUNICODE /I"lib\libgcrypt\include"
#LDFLAGS = /link /LIBPATH:"C:\Users\gardm\usr\local\lib\lib-libgcrypt-1.11.0-ucrt64-gcc"
LDFLAGS = /link /LIBPATH:"lib\libgcrypt\lib-libgcrypt-1.11.0-ucrt64-gcc"
LIBS = libgcrypt-20.lib libgpg-error-0.lib	

OUT_DIR = bin
TARGET = $(OUT_DIR)\puttyping_com.exe

SRCS = src\puttyping.c src\process.c \
	src\util\encryption.c src\util\hmac.c \
	src\libgcrypt_crypto.c

OBJS = src\puttyping.obj src\process.obj \
	src\util\encryption.obj src\util\hmac.obj \
	src\libgcrypt_crypto.obj

all: ensure_bin $(TARGET)

ensure_bin:
	if not exist $(OUT_DIR) mkdir $(OUT_DIR)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) /OUT:$(TARGET)

# Compile each .c file into an .obj file
.c.obj:
	$(CC) /c $(CFLAGS) $< /Fo$@

clean:
	del $(OUT_DIR)\*.exe $(OBJS)
