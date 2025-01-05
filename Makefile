CC = cl

CFLAGS = /LD /Gz /Fo:src/

LIBS = ws2_32.lib iphlpapi.lib

SRC = src\process.c

OUT = bin\process.dll

OUTPUTS = bin\process.dll src\process.obj

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) /link $(LIBS) /OUT:$(OUT)

	del process.exp
	del process.lib 

clean:
	del $(OUTPUTS)
