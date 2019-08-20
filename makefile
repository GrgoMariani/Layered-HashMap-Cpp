MAKEFILE_BUILDER = make -f makefile_builder.mk --no-print-directory


all: crt_hashmap stdlib_example


HASHMAP_SOURCES_CPP := HashMap/HashMap.cpp HashMap/Block.cpp Hashmap/Memory.cpp HashMap/Utils.cpp

crt_hashmap:
	@$(MAKEFILE_BUILDER) CPP_TO_COMPILE="$(HASHMAP_SOURCES_CPP) example.cpp" EXE_TARGET="crt_hashmap_example"


stdlib_example:
	@$(MAKEFILE_BUILDER) CPP_TO_COMPILE="example_stdlib.cpp" EXE_TARGET="stdlib_hashmap_example"

clean:
	@$(MAKEFILE_BUILDER) clean