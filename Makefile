PROJECT=vipers_public_backdoor

#ARCH=-m32
CXX=/usr/bin/x86_64-w64-mingw32-gcc
STD=c17
EXT=*.c

OUT_DIR=~/output
SOURCE_DIR=./src
RESOURCE_DIR=./resources

SYS_INCLUDES=-isystem/usr/lib/gcc/x86_64-w64-mingw32/9.3-win32/include \
		-isystem/usr/lib/gcc/x86_64-w64-mingw32/9.3-win32/include-fixed \
		-isystem/usr/x86_64-w64-mingw32/includes

INCLUDES=-Isrc \
		 -Iincludes

LIBS=-lws2_32

WARNINGS=-Wno-ignored-attributes \
		 -Wno-int-to-pointer-cast \
		 -Wno-ignored-attributes \

CXX_FLAGS=-s -static
EXTRA_CXX_FLAGS=$(CXX_FLAGS) -o $(OUT_DIR)/$(PROJECT).exe $(SOURCE_DIR)/$(EXT)

all:
	$(CXX) $(ARCH) -std=$(STD) $(EXTRA_CXX_FLAGS) \
	$(RESOURCE_DIR)/out/*.res \
	$(SYS_INCLUDES) \
	$(INCLUDES) \
	$(LIBS) \
	$(WARNINGS)
clean:
	rm ~/output/$(PROJECT).exe
