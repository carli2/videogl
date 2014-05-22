osmesa: osmesa.cpp scene.cpp
	clang++ osmesa.cpp scene.cpp `pkg-config --libs --cflags osmesa` -lGL -lSDL -o osmesa

output.mp4: osmesa
	./osmesa -x 1920 -y 1080 -o output.mp4
