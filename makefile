CFLAGS=-O2 -Werror -Wall -Wextra -Wpedantic 
FRAMEWORKS=-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
RAYLIB=libraylib.a
STD=-std=c99

snake: src/main.c src/main.h src/linked_list.h
	$(CC) $(STD) $(CFLAGS) $(FRAMEWORKS) $(RAYLIB) src/main.c -o snake

.PHONY: bundle
bundle: snake
	mkdir Snake.app
	mkdir Snake.app/Contents
	mkdir Snake.app/Contents/MacOS
	mkdir Snake.app/Contents/Resources
	cp snake Snake.app/Contents/MacOS

.PHONY: clean
clean:
	rm -rf snake
	rm -rf Snake.app
	/System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/LaunchServices.framework/Versions/A/Support/lsregister -f Snake.app