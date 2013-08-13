all:
	mkdir Game.app
	mkdir Game.app/Contents
	mkdir Game.app/Contents/MacOS
	gcc -o Game.app/Contents/MacOs/Game -I/Library/Frameworks/SDL.framework/Headers -I/Library/Frameworks/SDL_image.framework/Headers main.cpp SDLmain.m -framework SDL -framework SDL_image -framework Cocoa -framework OpenGL
clean:
	rm -r *.app