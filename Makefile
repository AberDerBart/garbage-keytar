TARGET = build/garbage-keytar.elf

.PHONY: flash clean distclean

$TARGET: *.c *.h CMakeLists.txt
	mkdir -p build
	cd build
	cmake -B build
	make -C build

flash: ${TARGET}
	sudo openocd -f openocd/picoprobe.cfg -f openocd/rp2040.cfg -c "program ${TARGET} verify reset exit"

clean:
	make -C build clean

distclean:
	rm -rf build
