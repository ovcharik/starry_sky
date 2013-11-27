CC=g++
CFLAGS=-std=c++0x `pkg-config glibmm-2.4 gtkmm-2.4 gdkmm-2.4 gtkglextmm-1.2 --cflags`
LDFLAGS=`pkg-config glibmm-2.4 gtkglextmm-1.2 gtkmm-2.4 gdkmm-2.4 --libs` -lSDL

OBJ=Obj
BIN=Bin
SRC=Src
DAT=Data

DEBUG=#-DDEBUG

SOURCES=$(wildcard $(SRC)/*.cc)

GLOBJECTS=$(SOURCES:$(SRC)%.cc=$(OBJ)%-gl.o)
WGLOBJECTS=$(SOURCES:$(SRC)%.cc=$(OBJ)%-wgl.o)

HEADERS=$(wildcard $(SRC)/*.h)

APP_NAME=StarrySky

all: opengl assets


opengl: $(GLOBJECTS) $(HEADERS)
	$(CC) $(GLOBJECTS) -o $(BIN)/$(APP_NAME)-gl $(LDFLAGS)

win-opengl: $(WGLOBJECTS) $(HEADERS)
	wineg++ $(WGLOBJECTS) -o $(BIN)/$(APP_NAME)-wgl $(LDFLAGS)

assets:
	[ -d $(BIN)/$(DAT) ] || cp -r $(DAT) $(BIN)/

clean:
	rm -r $(OBJ)/*.o $(BIN)/*

# Rules for objects

$(GLOBJECTS): $(OBJ)/%-gl.o: $(SRC)/%.cc
	$(CC) $(DEBUG) -c $< -o $@ $(CFLAGS)

$(WGLOBJECTS): $(OBJ)/%-wgl.o: $(SRC)/%.cc
	wineg++ $(CFLAGS) -D__MINGW32__ -c $< -o $@
