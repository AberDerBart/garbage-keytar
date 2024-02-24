SOURCE_FILES := $(wildcard *.bmp)
DEST_DIR := gen
DEST_FILES := $(patsubst %.bmp,${DEST_DIR}/%.h, ${SOURCE_FILES})

BIN2C := $(PWD)/../pico-ssd1306/tools/bin2c

.PHONY: all clean

all: ${DEST_FILES}

${DEST_DIR}/%.h: %.bmp
	mkdir -p gen
	convert $< -monochrome ${DEST_DIR}/$<
	cd ${DEST_DIR} && ${BIN2C} $< $(notdir $@)

clean:
	rm -rf gen
