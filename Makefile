CC ?= cc
C_FLAGS := -std=gnu11 $\
					 -O2 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -Iinclude -Ideps/orchestra/include -Ideps/termbox2
LD_FLAGS := -Ldeps/orchestra -lorchestra

DIRECTORIES := build deps
DEPENDENCIES := deps/orchestra deps/termbox2

OBJECT_FILES := build/main.o build/menu.o $\
								build/termbox2.o

all: ${DIRECTORIES} ${DEPENDENCIES} tfm

${DIRECTORIES}:
	-mkdir ${DIRECTORIES}

deps/orchestra:
	git -C deps clone https://github.com/asdfish/orchestra --depth=1
	make -C deps/orchestra

deps/termbox2:
	git -C deps clone https://github.com/termbox/termbox2 --depth=1

clean:
	-rm -rf ${DIRECTORIES}

${OBJECT_FILES}: build/%.o: src/%.c $(if $(wildcard include/%.h),include/%.h)
	${CC} -c $< ${C_FLAGS} -o $@

tfm: ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tfm
	strip tfm

.PHONY: all clean
