PREFIX = $(HOME)
# suffix for library. Use the versioni of Ubuntu
LIB_SUFFIX = _18

LIB_DIR = $(PREFIX)/lib
INC_DIR = $(PREFIX)/include

TARGET = libnrlmsise$(LIB_SUFFIX).so

HEADERS = nrlmsise-00.h
OBJS = nrlmsise-00.o nrlmsise-00_data.o
OPTS = -O3 -Wall -fPIC -I.
LIB_OPTS = -Wl,-R$(LIB_DIR)

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -shared -o $@ $(OBJS)

nrlmsise-00.o: nrlmsise-00.c $(HEADERS)
	g++ -c $< $(OPTS)

nrlmsise-00_data.o: nrlmsise-00_data.c $(HEADERS)
	g++ -c $< $(OPTS)

install: $(TARGET)
	@if [ ! -d $(LIB_DIR) ]; then \
	  mkdir -p $(LIB_DIR); \
	fi
	cp $(TARGET) $(LIB_DIR)
	@if [ ! -d $(INC_DIR) ]; then \
	  mkdir -p $(INC_DIR); \
	fi
	cp $(HEADERS) $(INC_DIR)

test_nrlmsise: test_nrlmsise.o
	g++ -o $@ test_nrlmsise.o $(LIB_OPTS) -L$(LIB_DIR) -lnrlmsise$(LIB_SUFFIX)

%.o: %.cpp
	g++ -c $< -Wall -O3

clean:
	rm -f *.o $(TARGET) test_nrlmsise
