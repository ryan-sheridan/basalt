# Root Makefile - delegates to the real Makefile in build/
# (the build Makefile uses paths relative to its own directory,
# so we cd into build to run it)

.PHONY: all build clean run

all: build

build:
	$(MAKE) -C build

clean:
	$(MAKE) -C build clean

run:
	$(MAKE) -C build run
