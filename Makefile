CC = cl
LD = link

CFLAGS = /LD /Gz /Fo:src/

LIBS = ws2_32.lib iphlpapi.lib

SRC = src\process.c

OUT = src\process.dll

OUTPUTS = src\process.dll src\process.obj

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) /link $(LIBS) /OUT:$(OUT)

	del process.exp
	del process.lib 

clean:
	del $(OUTPUTS)
