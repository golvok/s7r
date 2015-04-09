.SUFFIXES:
.PHONY: all clean

# remove ALL implicit rules
.SUFFIXES:
MAKEFLAGS+=" -r"

EXE_DIR=EXE/
OBJ_DIR=OBJ/
DEPS_DIR=DEPS/

# CPP = clang++ -stdlib=libstdc++
CPP = g++

CPPFLAGS += -Wall -Wextra -pedantic -Weffc++ -Werror -std=c++11 -ggdb

LINK_FLAGS = \
	$(shell pkg-config --libs gtk+-3.0) \
	$(shell pkg-config --libs cairo)

INCLUDE_FLAGS = \
	$(shell pkg-config --cflags gtk+-3.0) \
	$(shell pkg-config --cflags cairo) \
	-I .

# keep .o files
.PRECIOUS: $(OBJ_DIR)%.o

# define source directories
SOURCE_DIRS = draw/ sim/ util/ .

# compute all directories that might need creation
DIRS=$(EXE_DIR) $(OBJ_DIR) $(DEPS_DIR) \
	$(addprefix $(OBJ_DIR),$(SOURCE_DIRS)) \
	$(addprefix $(DEPS_DIR),$(SOURCE_DIRS))

# define executables
EXES=$(EXE_DIR)s7r

all: $(EXES)

# add more dependencies here:
$(EXE_DIR)s7r: \
	$(OBJ_DIR)draw/window.o \
	$(OBJ_DIR)sim/particle.o \
	$(OBJ_DIR)sim/sim.o \
	$(OBJ_DIR)sim/mover.o \
	$(OBJ_DIR)util/geometry.o \
	$(OBJ_DIR)main.o

# include all the dependency files, if any exist
EXISTING_DEP_FILES = \
	$(foreach dir,$(SOURCE_DIRS), \
		$(wildcard $(DEPS_DIR)$(dir)*.d) \
	)
ifneq (,$(EXISTING_DEP_FILES))
include $(EXISTING_DEP_FILES)
endif

# compile .c++ to .o
# second CC line generates the initial dependency file
# first sed line adds $(OBJ_DIR) prefixes to the dependency file,
# second one adds stub rules for each depended on file (make might
# complain with generated d
.SECONDEXPANSION:
$(OBJ_DIR)%.o: %.c++ | $(OBJ_DIR)$$(dir %) $(DEPS_DIR)$$(dir %)
	$(CPP) -c  $< -o  $@ $(CPPFLAGS) $(INCLUDE_FLAGS)
	$(CPP) -MM $< -MF $(DEPS_DIR)$<.d.tmp $(CPPFLAGS) $(INCLUDE_FLAGS)
	@sed -e 's|.*:|$(OBJ_DIR)$<.o:|' < $(DEPS_DIR)$<.d.tmp > $(DEPS_DIR)$<.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPS_DIR)$<.d.tmp | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPS_DIR)$<.d
	@rm -f $(DEPS_DIR)$<.d.tmp

# compile *.o's into an executable
$(EXE_DIR)%: | $(EXE_DIR)
	$(CPP) $^ -o $@ $(CPPFLAGS) $(LINK_FLAGS)

$(DIRS):
	mkdir -p $@

clean:
	-rm -f $(EXES);
	-if [ -a $(EXE_DIR)  ]; then rmdir --ignore-fail-on-non-empty $(EXE_DIR);  fi;

	for subdir in $(SOURCE_DIRS); do \
		if [ -a $(DEPS_DIR)$${subdir} ]; then \
			deps_subdir=$$(readlink --canonicalize $(DEPS_DIR)$${subdir})/; \
			echo $${deps_subdir}; \
			rm -f $${deps_subdir}*.d; \
			rmdir --ignore-fail-on-non-empty $${deps_subdir}; \
		fi; \
		if [ -a $(OBJ_DIR)$${subdir} ]; then \
			objs_subdir=$$(readlink --canonicalize $(OBJ_DIR)$${subdir})/; \
			echo $${objs_subdir}; \
			rm -f $${objs_subdir}*.o; \
			rmdir --ignore-fail-on-non-empty $${objs_subdir}; \
		fi; \
	done
