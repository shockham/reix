all:
	mkdir Game.app
	mkdir Game.app/Contents
	mkdir Game.app/Contents/MacOS
	gcc -o Game.app/Contents/MacOs/Game -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers main.cpp -framework SDL2 -framework SDL2_image -framework Cocoa -framework OpenGL
	# gcc -o Game -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers main.cpp -framework SDL2 -framework SDL2_image -framework Cocoa -framework OpenGL
clean:
	rm -r *.app
	# rm Game
