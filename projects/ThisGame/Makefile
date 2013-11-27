all:
	g++ -o Reix -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers main.cpp -framework SDL2 -framework SDL2_image -framework Cocoa -framework OpenGL
clean:
	rm -r *.app
	rm Reix
app:
	mkdir Reix.app
	mkdir Reix.app/Contents
	mkdir Reix.app/Contents/MacOS
	cp -r ./stuff ./Reix.app/Contents/MacOS
	g++ -o Reix.app/Contents/MacOs/Reix -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers main.cpp -framework SDL2 -framework SDL2_image -framework Cocoa -framework OpenGL
