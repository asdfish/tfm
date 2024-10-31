CC := gcc

C_FLAGS := -std=gnu11 $\
					 -Og -g -march=native -pipe $\
					 -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter $\
					 -I. -Iinclude -Ideps/orchestra/include -Ideps/termbox2
LD_FLAGS := -Ldeps/orchestra -lorchestra

DIRECTORIES := deps
DEPENDENCIES := deps/orchestra deps/termbox2

HEADER_FILES := $(shell find -name '*.h' -not -path './deps/*')
PROCESSED_HEADER_FILES := $(subst .h,$\
														$(if $(findstring clang,${CC}),$\
															.h.pch,$\
															.h.gch$\
														),$\
													${HEADER_FILES})

SOURCE_FILES := $(shell find -name '*.c' -not -path './deps/*')
OBJECT_FILES := $(patsubst ./src/%.c,./build/%.o,${SOURCE_FILES})

all: ${DIRECTORIES} ${DEPENDENCIES} tfm

${DIRECTORIES}:
	$(foreach DIRECTORY,$\
		${DIRECTORIES},$\
		$(if $(wildcard ${DIRECTORY}),,$\
			$(shell mkdir ${DIRECTORY})))

deps/orchestra:
	git -C deps clone https://github.com/asdfish/orchestra --depth=1
	$(MAKE) -C deps/orchestra

deps/termbox2:
	git -C deps clone https://github.com/termbox/termbox2 --depth=1

clean:
	$(foreach DIRECTORY,$\
		${DIRECTORIES},$\
		$(if $(wildcard ${DIRECTORY}),$\
			$(shell rm -rf ${DIRECTORY})))
ifneq (, $(wildcard tfm))
	rm -f tfm
endif
	$(foreach PROCESSED_HEADER_FILE,$\
		${PROCESSED_HEADER_FILES},$\
		$(if $(wildcard ${PROCESSED_HEADER_FILE}),$\
			$(shell rm ${PROCESSED_HEADER_FILE})))

%.gch: %
	${CC} -c $< ${C_FLAGS}

%.pch: %
	${CC} -c $< ${C_FLAGS}

build/%.o: src/%.c
ifeq (, $(wildcard $(dir $<)))
	mkdir -p $(dir $<)
endif
	${CC} -c $< ${C_FLAGS} -o $@

tfm: ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tfm

.PHONY: all clean
