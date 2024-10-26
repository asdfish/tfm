COMPILE_FLAGS := -O2 -march=native -pipe $\
								 -Wall -Wextra -Wpedantic $\
								 -I. -Iinclude -Ideps/orchestra/include -Ideps/termbox2

CC ?= cc
CXX ?= c++

C_FLAGS := -std=gnu11 ${COMPILE_FLAGS}
CXX_FLAGS := -std=c++20 ${COMPILE_FLAGS} -Wno-missing-field-initializers
LD_FLAGS := -Ldeps/orchestra -lorchestra

DIRECTORIES := build deps
DEPENDENCIES := deps/orchestra deps/termbox2

C_OBJECT_FILES := build/menu.o build/utils.o $\
									build/termbox2.o

CXX_OBJECT_FILES := build/main.o

all: ${DIRECTORIES} ${DEPENDENCIES} tfm

${DIRECTORIES}:
	-mkdir ${DIRECTORIES}

deps/orchestra:
	git -C deps clone https://github.com/asdfish/orchestra --depth=1
	$(MAKE) -C deps/orchestra

deps/termbox2:
	git -C deps clone https://github.com/termbox/termbox2 --depth=1

clean:
	-rm -rf ${DIRECTORIES}
	-rm tfm

${C_OBJECT_FILES}: build/%.o: src/%.c
	${CC} -c $< ${C_FLAGS} -o $@

${CXX_OBJECT_FILES}: build/%.o: src/%.cpp
	${CXX} -c $< ${CXX_FLAGS} -o $@

tfm: ${C_OBJECT_FILES} ${CXX_OBJECT_FILES}
	${CC} ${C_OBJECT_FILES} ${CXX_OBJECT_FILES} ${LD_FLAGS} -o tfm
	strip tfm

.PHONY: all clean
