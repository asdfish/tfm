COMPILE_FLAGS := 

CC ?= cc

C_FLAGS := -std=gnu11 $\
					 -Og -g -march=native -pipe $\
					 -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter $\
					 -I. -Iinclude -Ideps/orchestra/include -Ideps/termbox2
LD_FLAGS := -Ldeps/orchestra -lorchestra

DIRECTORIES := build deps
DEPENDENCIES := deps/orchestra deps/termbox2

OBJECT_FILES := build/bind_functions.o build/main.o build/menu.o build/utils.o build/tfm.o $\
								build/termbox2.o

all: ${DIRECTORIES} ${DEPENDENCIES} tfm

${DIRECTORIES}:
	$(foreach DIRECTORY,$\
		${DIRECTORIES},$\
		$(if $(wildcard ${DIRECTORY}),,$\
			$(shell mkdir ${DIRECTORY})$\
		)$\
	)

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

${OBJECT_FILES}: build/%.o: src/%.c
	${CC} -c $< ${C_FLAGS} -o $@
	# strip --strip-unneeded $@

tfm: ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tfm
	# strip -s tfm

.PHONY: all clean
