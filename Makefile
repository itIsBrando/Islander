
CP := /opt/gbdk/bin
OUTPUT_NAME := islander
SOURCE_FOLDERS := src src/world src/data src/entity src/item src/gui

# Nothing should require editting down below

SOURCES := $(foreach dir, $(SOURCE_FOLDERS), $(wildcard $(dir)/*.c))

build:
	@$(CP)/lcc -Wa-l -Wl-m -Wl-j -Wm-yc -o $(OUTPUT_NAME).gb $(SOURCES)
