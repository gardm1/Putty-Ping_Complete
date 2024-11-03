CC = cl
CFLAGS = /nologo /W3 /EHsc /TC /D_UNICODE /DUNICODE -I. 

OUT_DIR = bin
TARGET = $(OUT_DIR)/puttyping_com.exe

# Header files
HFILE1 = src/process.h
HFILE2 = src/dataHandler.h

INCLUDES = $(HFILE1) $(HFILE2)

# Source files
CFILE1 = src/puttyping.c
CFILE2 = src/process.c
CFILE3 = src/dataHandler.c

SOURCES = $(CFILE1) $(CFILE2) $(CFILE3)

# Rules
all: $(TARGET)

$(TARGET): $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES) /Fe$(TARGET)

clean:
	DEL $(TARGET)
	DEL *.obj