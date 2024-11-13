CC = cl
CFLAGS = /nologo /W3 /D_UNICODE /DUNICODE /I"C:\Users\gardm\usr\include\libgcrypt" 
LDFLAGS = /link /LIBPATH:"C:\Users\gardm\usr\local\lib\lib-libgcrypt-1.11.0-ucrt64-gcc"
LIBS = libgcrypt-20.lib libgpg-error-0.lib
#LIBS = -lgcrypt -lgpg-error	

OUT_DIR = bin
TARGET = $(OUT_DIR)\puttyping_com.exe


SRCS = src\puttyping.c src\process.c src\dataHandler.c
OBJS = src\puttyping.obj src\process.obj src\dataHandler.obj

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) /OUT:$(TARGET)

# Compile each .c file into an .obj file
.c.obj:
	$(CC) /c $(CFLAGS) $< /Fo$@

# Clean up
clean:
	del $(OUT_DIR)\*.exe $(OBJS)
