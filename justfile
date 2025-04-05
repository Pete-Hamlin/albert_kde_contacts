@default: build install

[working-directory: 'build']
install:
	cp contacts_kde.so $HOME/.local/lib/albert/

@uninstall:
	rm  $HOME/.local/lib/albert/contacts_kde.so


[working-directory: 'build']
@build: init
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. -DCMAKE_PREFIX_PATH=/usr/lib/$(gcc -dumpmachine)/cmake/
	cmake --build . -j$(nproc)
	make

@init:
	[ ! -d build ] && mkdir build/ || exit 0

@clean:
	rm -rf build/

