#CC = cl
#CFLAGS = /nologo /W3 /EHsc /TC /D_UNICODE /DUNICODE -I. -I"C:/msys64/ucrt64/include"
#LDFLAGS = /link /LIBPATH:"C:/msys64/ucrt64/lib" gcrypt.lib gpg-error.lib

#CFLAGS = /nologo /W3 /EHsc /TC /D_UNICODE /DUNICODE -I.
####PATH=C:\msys64\ucrt64\bin;%PATH%

#OUT_DIR = bin
#TARGET = $(OUT_DIR)/puttyping_com.exe

# Header files
#HFILE1 = src/process.h
#HFILE2 = src/dataHandler.h

#HEADERS = $(HFILE1) $(HFILE2)

# Source files
#CFILE1 = src/puttyping.c
#CFILE2 = src/process.c
#CFILE3 = src/dataHandler.c

#SOURCES = $(CFILE1) $(CFILE2) $(CFILE3)

# Rules
#all: $(TARGET)

#$(TARGET): $(SOURCES) $(HEADERS)
#	$(CC) $(CFLAGS) $(SOURCES) /Fe$(TARGET)
	
#clean:
#	DEL $(TARGET)
#	DEL *.obj

# Paths to the MSYS2 UCRT64 directories
#MSYS2_PATH = C:\msys64
#GCC = $(MSYS2_PATH)\ucrt64\bin\gcc.exe
#INCLUDES = -I"$(MSYS2_PATH)\ucrt64\include"
#LIBS = -L"$(MSYS2_PATH)\ucrt64\lib" -lgcrypt -lgpg-error -lws2_32 -liphlpapi -luser32 -lkernel32
#LIBRARY_PATH = $(MSYS2_PATH)\ucrt64\bin

# Target output binary
#OUT_DIR = bin
#TARGET = $(OUT_DIR)\puttyping_com.exe

# Source and object files
#SRCS = src\puttyping.c src\process.c src\dataHandler.c
#OBJS = src\puttyping.o src\process.o src\dataHandler.o

# Create output directory if it doesn't exist and build the target
#all: $(TARGET)

# Linking target with verbose output
#$(TARGET): $(OBJS)
#	$(GCC) -v $(OBJS) -o $(TARGET) $(INCLUDES) $(LIBS) -L"$(LIBRARY_PATH)"

# Compile each .c file into an object file
#src\puttyping.o: src\puttyping.c
#	$(GCC) -c src\puttyping.c -o src\puttyping.o $(INCLUDES)

#src\process.o: src\process.c
#	$(GCC) -c src\process.c -o src\process.o $(INCLUDES)

#src\dataHandler.o: src\dataHandler.c
#	$(GCC) -c src\dataHandler.c -o src\dataHandler.o $(INCLUDES)

# Clean up
#clean:
#	del $(TARGET) $(OBJS)

CC = cl
CFLAGS = /I"C:\Users\gardm\usr\include\libgcrypt" /D"_WIN32"
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
