

all: clean release

clean: cleand cleanr

cleand:
	rm -rf build_debug

cleanr:
	rm -rf build_release

debug: cleand
	mkdir build_debug
	cd build_debug/ && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=install ../ && make -j2 && make install

release: cleanr
	mkdir build_release
	cd build_release/ && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install ../ && make -j2 && make install