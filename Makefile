SOURCES=src/osmesa.cpp src/videohelpers.cpp

all: osmesa *.so

osmesa: $(SOURCES)
	clang++ $(SOURCES) `pkg-config --libs --cflags osmesa` -lGL -lSDL -o osmesa -ldl -g -Iinclude -rdynamic

output.mp4: osmesa
	./osmesa -x 1920 -y 1080 -o output.mp4

hello.so: examples/hello.cpp
	gcc examples/hello.cpp -shared -ohello.so -fPIC -Iinclude
grid.so: examples/grid.cpp
	gcc examples/grid.cpp -shared -ogrid.so -fPIC -Iinclude
