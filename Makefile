CC = cl
CFLAGS = /nologo /W3 /EHsc /TC /D_UNICODE /DUNICODE -I. 

OUT_DIR = bin
TARGET = $(OUT_DIR)/puttyping_com.exe

# Source files
CFILE1 = src/puttyping.c
CFILE2 = src/process.c

SOURCES = $(CFILE1) $(CFILE2)

# Header files
HFILE1 = src/process.h

INCLUDES = $(HFILE1)

# Rules
all: $(TARGET)

$(TARGET): $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES) /Fe$(TARGET)

clean:
	DEL $(TARGET)
	DEL *.obj