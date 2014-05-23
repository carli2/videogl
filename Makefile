SOURCES=src/osmesa.cpp src/videohelpers.cpp

osmesa: $(SOURCES)
	clang++ $(SOURCES) `pkg-config --libs --cflags osmesa` -lGL -lSDL -o osmesa -ldl -g -Iinclude

output.mp4: osmesa
	./osmesa -x 1920 -y 1080 -o output.mp4

hello.so: examples/hello.cpp
	gcc examples/hello.cpp -shared -ohello.so -fPIC -Iinclude
