
CP := /opt/gbdk/bin

SOURCE_FOLDERS := src src/world src/data src/entity src/item src/gui


SOURCES := $(foreach dir, $(SOURCE_FOLDERS), $(wildcard $(dir)/*.c))

build:
	@$(CP)/lcc -Wa-l -Wl-m -Wl-j -Wm-yc -o main.gb $(SOURCES)
