CC := gcc

C_FLAGS := -std=gnu11 $\
					 -O2 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter $\
					 -I. -Iinclude -Ideps/orchestra/include -Ideps/termbox2
LD_FLAGS := -Ldeps/orchestra -lorchestra

PROCESSED_HEADER_FILES := $(subst .h,$\
														$(if $(findstring clang,${CC}),$\
															.h.pch,$\
															.h.gch$\
														),$\
														$(shell find -name '*.h' -not -path './deps/*'))

OBJECT_FILES := $(patsubst ./src/%.c,$\
									./build/%.o,$\
									$(shell find -name '*.c' -not -path './deps/*'))

all: tfm

clean:
ifneq (,$(wildcard tfm))
	rm -f tfm
endif
ifneq (,$(wildcard build/*.o))
	rm build/*.o
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
ifeq (,$(wildcard $(dir $@)))
	mkdir -p $(dir $@)
endif
	${CC} -c $< ${C_FLAGS} -o $@

deps/orchestra/liborchestra.a:
	$(MAKE) -C deps/orchestra

tfm: deps/orchestra/liborchestra.a ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tfm

.PHONY: all clean
