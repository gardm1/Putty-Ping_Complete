CC = cl
CFLAGS = /nologo /W3 /EHsc /TC /D_UNICODE /DUNICODE -I.  # /TC forces C compilation

OUT_DIR = bin
TARGET = $(OUT_DIR)/puttyping_com.exe

SOURCES = src/puttyping.c # Use forward slashes

# Rules
all: $(TARGET)

$(TARGET): $(SOURCES) src\process.h
	$(CC) $(CFLAGS) $(SOURCES) /Fe$(TARGET)

clean:
	DEL $(TARGET)
	DEL *.obj