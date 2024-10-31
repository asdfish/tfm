CC := gcc

C_FLAGS := -std=gnu11 $\
					 -Og -g -march=native -pipe $\
					 -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter $\
					 -I. -Iinclude -Ideps/cector -Ideps/orchestra/include -Ideps/termbox2
LD_FLAGS := -Ldeps/orchestra -lorchestra

DIRECTORIES := build deps build/utils
DEPENDENCIES := deps/cector deps/orchestra deps/termbox2

PROCESSED_HEADER_FILES := include/bind_functions.h.gch include/commands.h.gch include/command_line.h.gch include/macros.h.gch include/menu.h.gch include/tfm.h.gch $\
													include/utils/filesystem.h.gch include/utils/string.h.gch include/utils/ui.h.gch

OBJECT_FILES := build/bind_functions.o build/commands.o build/command_line.o build/main.o build/menu.o build/tfm.o $\
								build/utils/filesystem.o build/utils/string.o build/utils/ui.o $\
								build/termbox2.o

all: check_clang ${DIRECTORIES} ${DEPENDENCIES} tfm

check_clang:
ifeq (clang,${CC})
	$(eval PROCESSED_HEADER_FILES := $(subst .h,.h.pch,${PROCESSED_HEADER_FILES}))
endif

${DIRECTORIES}:
	$(foreach DIRECTORY,$\
		${DIRECTORIES},$\
		$(if $(wildcard ${DIRECTORY}),,$\
			$(shell mkdir ${DIRECTORY})$\
		)$\
	)

deps/cector:
	git -C deps clone https://github.com/asdfish/cector --depth=1

deps/orchestra:
	git -C deps clone https://github.com/asdfish/orchestra --depth=1
	$(MAKE) -C deps/orchestra

deps/termbox2:
	git -C deps clone https://github.com/termbox/termbox2 --depth=1

clean:
	$(foreach DIRECTORY,$\
		${DIRECTORIES},$\
		$(if $(wildcard ${DIRECTORY}),$\
			$(shell rm -rf ${DIRECTORY})$\
		)$\
	)
ifneq (, $(wildcard tfm))
	rm -f tfm
endif
ifneq (, $(wildcard include/*.gch))
	rm include/*.gch
endif
ifneq (, $(wildcard include/*.pch))
	rm include/*.pch
endif

include/%.gch: include/%
	${CC} -c $< ${C_FLAGS}

include/%.pch: include/%.gch
	${CC} -c $< ${C_FLAGS}

build/%.o: src/%.c
	${CC} -c $< ${C_FLAGS} -o $@

tfm: ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tfm

.PHONY: all check_clang clean
