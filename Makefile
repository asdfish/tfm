CC := gcc

C_FLAGS := -std=gnu11 $\
					 -Og -g -march=native -pipe $\
					 -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter $\
					 -I. -Iinclude -Ideps/orchestra/include -Ideps/termbox2
LD_FLAGS := -Ldeps/orchestra -lorchestra

HEADER_FILES := $(shell find -name '*.h' -not -path './deps/*')
PROCESSED_HEADER_FILES := $(subst .h,$\
														$(if $(findstring clang,${CC}),$\
															.h.pch,$\
															.h.gch$\
														),$\
													${HEADER_FILES})

SOURCE_FILES := $(shell find -name '*.c' -not -path './deps/*')
OBJECT_FILES := $(patsubst ./src/%.c,./build/%.o,${SOURCE_FILES})

all: ${DEPENDENCIES} tfm

clean:
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
