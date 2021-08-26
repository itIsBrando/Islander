# Incredibly pointless Makefile alternative because school's public computers do not allow for root access
import os


SOURCES = "src src/world src/data src/entity src/item src/gui"
COMPILER_PATH = "/Users/btk5203/Documents/gbdk/bin"
OUTPUT_NAME = "islander"
FLAGS = "-Wa-l -Wl-m -Wl-j -Wm-yc -o"








FLAGS += " " + OUTPUT_NAME + ".gb "

for i in SOURCES.split(" "):
    FLAGS += i + "/*.c "


# runs a bash shell command
def runShellCommand(command):
    os.system(command)

c = COMPILER_PATH + "/lcc " + FLAGS + ""
print(c)
runShellCommand(c)
