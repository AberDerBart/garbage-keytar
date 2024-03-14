SOURCE_FILES := $(wildcard *.bmp)
DEST_DIR := gen
DEST_FILES := $(patsubst %.bmp,${DEST_DIR}/%.h, ${SOURCE_FILES})

.PHONY: all clean

all: ${DEST_FILES}

${DEST_DIR}/%.h: %.bmp
	mkdir -p gen
	convert $< -monochrome ${DEST_DIR}/$<
	cd ${DEST_DIR} && bin2c $< $(notdir $@)

clean:
	rm -rf gen
