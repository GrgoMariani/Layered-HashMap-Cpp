.PRECIOUS: %/.

HASHMAP_SOURCES_CPP = HashMap/HashMap.cpp HashMap/Block.cpp Hashmap/Memory.cpp HashMap/Utils.cpp
WRAPPER_SOURCES_C	= Wrapper/Useful/getRSS.c Wrapper/Useful/ansi_escapes.c
WRAPPER_SOURCES_CPP = Wrapper/benchmark.cpp   Wrapper/wrapper_main.cpp

ifeq ($(OS),Windows_NT)
# Windows + minGW
ASM_FLAGS =
LINKER_FLAGS = -lpsapi -static-libgcc -static-libstdc++
else
# Linux
ASM_FLAGS = -felf
endif

ANSI_RED 		:= "\033[1;31m"
ANSI_CYAN 		:= "\033[1;36m"
ANSI_YELLOW 	:= "\033[1;33m"
ANSI_GREEN 		:= "\033[1;32m"
ANSI_DEFAULT 	:= "\033[0m"

build_dir 		:= build
obj_dir			:= $(build_dir)/objs
dep_dir			:= $(build_dir)/deps
orig_o_dir 		:= $(obj_dir)/orig
orig_d_dir 		:= $(dep_dir)/orig

ASM_TO_COMPILE 	+=
C_TO_COMPILE 	+= $(WRAPPER_SOURCES_C)
CPP_TO_COMPILE 	+= $(WRAPPER_SOURCES_CPP) $(HASHMAP_SOURCES_CPP) example.cpp	# in case you want to compare against std::unordered_map<> comment this line
#CPP_TO_COMPILE 	+= $(WRAPPER_SOURCES_CPP) example_stdlib.cpp				# and uncomment this one
EXE_TARGET		= execute_me

TARGET			:= $(build_dir)/$(EXE_TARGET)

# ANSI color palette


# Various targets
all: $(TARGET)

clean:
	@echo -e $(ANSI_RED) ------------- Cleaning -------------- $(ANSI_DEFAULT)
	@rm -rf $(build_dir)

%/.:
	@echo -e $(ANSI_CYAN) ----------- Creating Directory : $(@D) ----------------- $(ANSI_DEFAULT)
	@mkdir -p $(@D)

ORIG_C_SOURCES		:= $(C_TO_COMPILE)
ORIG_CPP_SOURCES	:= $(CPP_TO_COMPILE)

ORIG_C_DEPS			:= $(ORIG_C_SOURCES:%.c=$(orig_d_dir)/c/%.d)
ORIG_C_OBJS			:= $(ORIG_C_SOURCES:%.c=$(orig_o_dir)/c/%.o)
ORIG_CPP_DEPS		:= $(ORIG_CPP_SOURCES:%.cpp=$(orig_d_dir)/cpp/%.d)
ORIG_CPP_OBJS		:= $(ORIG_CPP_SOURCES:%.cpp=$(orig_o_dir)/cpp/%.o)

ORIG_ASM_OBJS		:= $(ASM_TO_COMPILE:%.asm=$(orig_o_dir)/asm/%.o)

INCLUDE_DIRS 		:=

C_FLAGS 	:= -std=c11	  -fPIC -Wall -O3
CXX_FLAGS 	:= -std=c++11 -fPIC -Wall -O3 -Weffc++ 

DEPFLAGS_DEFAULT	 = -MT $@ -MMD -MP
DEPFLAGS_C_ORIG 	 = $(DEPFLAGS_DEFAULT) -MF $(orig_d_dir)/c/$*.d
DEPFLAGS_CPP_ORIG 	 = $(DEPFLAGS_DEFAULT) -MF $(orig_d_dir)/cpp/$*.d


DEPFILES := $(ORIG_CPP_SOURCES:%.cpp=$(orig_d_dir)/cpp/%.d) $(ORIG_C_SOURCES:%.c=$(orig_d_dir)/c/%.d)
$(DEPFILES):
include $(wildcard $(DEPFILES))


.SECONDEXPANSION:

#  ------------------- Compile objects -------------------
$(orig_o_dir)/asm/%.o : %.asm makefile | $$(@D)/.
	@echo -e $(ANSI_YELLOW) ----------- Assembling : $@ ----------------- $(ANSI_DEFAULT)
	nasm -w+all $(ASM_FLAGS) $< -o $@

$(orig_o_dir)/c/%.o : %.c makefile | $$(@D)/. $(orig_d_dir)/c/$$(*D)/.
	@echo -e $(ANSI_YELLOW) ----------- Compiling : $@ ----------------- $(ANSI_DEFAULT)
	gcc $(C_FLAGS) $(DEPFLAGS_C_ORIG) $(INCLUDE_DIRS) -c $< -o $@

$(orig_o_dir)/cpp/%.o : %.cpp makefile | $$(@D)/. $(orig_d_dir)/cpp/$$(*D)/.
	@echo -e $(ANSI_YELLOW) ----------- Compiling : $@ ----------------- $(ANSI_DEFAULT)
	g++ $(CXX_FLAGS) $(DEPFLAGS_CPP_ORIG) $(INCLUDE_DIRS) -c $< -o $@

#  ------------------- link final target -------------------
$(TARGET): $(ORIG_ASM_OBJS) $(ORIG_C_OBJS) $(ORIG_CPP_OBJS) | $$(@D)/.
	@echo -e $(ANSI_GREEN) ----------- Linking : $@ ----------------- $(ANSI_DEFAULT)
	g++ $(ORIG_ASM_OBJS) $(ORIG_C_OBJS) $(ORIG_CPP_OBJS) $(LINKER_FLAGS) -o $@ -s
	@echo -e $(ANSI_RED) ----------- DONE: Target $@ ----------- $(ANSI_DEFAULT)
