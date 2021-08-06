
CP := /opt/gbdk/bin

build:
	@$(CP)/lcc -Wa-l -Wl-m -Wl-j -Wm-yc -o main.gb src/*.c src/world/*.c src/data/*.c
