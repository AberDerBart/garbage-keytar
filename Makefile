TARGET = build/keytar.elf

.PHONY: flash clean distclean

$TARGET: *.c *.h CMakeLists.txt
	mkdir -p build
	cd build
	cmake -B build
	make -C build

flash: ${TARGET}
	sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program ${TARGET} verify reset exit"

clean:
	make -C build clean

distclean:
	rm -rf build
