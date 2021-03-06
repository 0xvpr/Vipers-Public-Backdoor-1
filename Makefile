PROJECT = vipers_public_backdoor.exe

CC      = i686-w64-mingw32-gcc
CFLAGS  = -std=c99 -Os -fno-ident -Wall -Wextra

LD      = i686-w64-mingw32-gcc
LDFLAGS = -s -lws2_32

BIN     = bin
RES     = res
SRC     = src
OBJ     = build
BUILD   = $(OBJ)
SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SOURCES))

INCLUDE = $(addprefix -I,includes)

all: $(PROJECT)

$(PROJECT): $(BIN) $(BUILD) $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BIN)/$(PROJECT)

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD):
	mkdir -p $@

$(BIN):
	mkdir -p $@

clean:
	rm -fr $(BUILD)/*
	rm -f $(BIN)/$(PROJECT)
