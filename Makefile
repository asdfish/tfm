CC ?= gcc

C_FLAGS := -std=gnu11 $\
					 -O2 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter $\
					 -I. -Iinclude -Ideps/orchestra/include -Ideps/termbox2
LD_FLAGS := -Ldeps/orchestra -lorchestra

PROCESS_HEADER_FILES := yes
PROCESSED_HEADER_FILES := $(if ${PROCESS_HEADER_FILES},$\
														$(subst .h,$\
															$(if $(findstring clang,${CC}),$\
																.h.pch,$\
																.h.gch),$\
															$(shell find -name '*.h' -not -path './deps/*')))

OBJECT_FILES := $(patsubst ./src/%.c,$\
									./build/%.o,$\
									$(shell find -name '*.c' -not -path './deps/*'))

TFM_REQUIREMENTS := deps/orchestra/liborchestra.a ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}

define REMOVE
$(if $(wildcard $(1)),$\
	rm $(1))

endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(1),$\
	$(call REMOVE,${ITEM}))

endef

all: tfm

clean:
	$(call REMOVE_LIST,${TFM_REQUIREMENTS} tfm)

%.gch: %
	${CC} -c $< ${C_FLAGS}

%.pch: %
	${CC} -c $< ${C_FLAGS}

build/%.o: src/%.c
	${CC} -c $< ${C_FLAGS} -o $@

deps/orchestra/liborchestra.a:
	$(MAKE) -C deps/orchestra

tfm: ${TFM_REQUIREMENTS}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tfm

.PHONY: all clean
