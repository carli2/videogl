osmesa: osmesa.cpp scene.cpp
	clang++ osmesa.cpp scene.cpp `pkg-config --libs --cflags osmesa` -lGL -lSDL -o osmesa

output.mp4: osmesa
	./osmesa -w 1920 -h 1080 -o output.mp4
