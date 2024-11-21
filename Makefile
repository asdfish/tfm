CC ?= gcc
CFLAGS ?= -O2 -march=native -pipe
COMMONFLAGS := -std=gnu11 $\
							 -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter $\
							 -I. -Iinclude -Ideps/orchestra/include -Ideps/termbox2
LDFLAGS := ${CFLAGS} ${COMMONFLAGS} $\
					 -Ldeps/orchestra -lorchestra

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
define REMOVE_DEPENDENCY
$(MAKE) -C $(dir $(1)) clean

endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(1),$\
	$(if $(findstring .a,${ITEM}),$\
		$(call REMOVE_DEPENDENCY,${ITEM}),$\
		$(call REMOVE,${ITEM})))

endef
define COMPILE
${CC} -c $1 ${CFLAGS} ${COMMONFLAGS} -o $2
endef

all: tfm

clean:
	$(call REMOVE_LIST,${TFM_REQUIREMENTS} tfm)

%.gch: %
	$(call COMPILE,$<,$@)
%.pch: %
	$(call COMPILE,$<,$@)

build/%.o: src/%.c
	$(call COMPILE,$<,$@)

%.a:
	CFLAGS='${CFLAGS}' $(MAKE) -C $(dir $@)

tfm: ${TFM_REQUIREMENTS}
	${CC} ${OBJECT_FILES} ${LDFLAGS} -o tfm

.PHONY: all clean
