CFLAGS=-std=c++0x
LDFLAGS=

OBJ=Obj
BIN=Bin
SRC=Src
DAT=Data

DEBUG=#-DDEBUG
APP_NAME=StarrySky

GENERAL_SOURCES=$(wildcard $(SRC)/general/*.cc)

GL_SOURCES=$(GENERAL_SOURCES) $(wildcard $(SRC)/opengl/*.cc)
GL_OBJECTS=$(GL_SOURCES:$(SRC)%.cc=$(OBJ)%-gl.o)

DX_SOURCES=$(GENERAL_SOURCES) $(wildcard $(SRC)/directx/*.cc)
DX_OBJECTS=$(DX_SOURCES:$(SRC)%.cc=$(OBJ)%-dx.o)

all: create_dirs assets opengl

create_dirs:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)/general $(OBJ)/opengl $(OBJ)/directx

assets:
	[ -d $(BIN)/$(DAT) ] || cp -r $(DAT) $(BIN)/

clean:
	rm -r $(OBJ) $(BIN)


# Creating bin
opengl: create_dirs assets $(GL_OBJECTS)
	g++ $(GL_OBJECTS) -o $(BIN)/$(APP_NAME)-gl $(LDFLAGS)\
		`pkg-config glibmm-2.4 gtkglextmm-1.2 gtkmm-2.4 gdkmm-2.4 --libs` -lSDL

directx: create_dirs assets $(DX_OBJECTS)
	wineg++ $(DX_OBJECTS) -o $(BIN)/$(APP_NAME)-dx $(LDFLAGS) -ld3d9 -ld3dx9


# Rules for objects
$(GL_OBJECTS): $(OBJ)/%-gl.o: $(SRC)/%.cc
	g++ $(DEBUG) -c $< -o $@ $(CFLAGS)\
		`pkg-config glibmm-2.4 gtkmm-2.4 gdkmm-2.4 gtkglextmm-1.2 --cflags`

$(DX_OBJECTS): $(OBJ)/%-dx.o: $(SRC)/%.cc
	wineg++ $(DEBUG) -c $< -o $@ $(CFLAGS)
