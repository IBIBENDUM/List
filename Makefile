include colors.mk

CXX 		:=	clang++
DEDFLAGS	:=	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal				\
				-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192	\
				-Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla		\
				-fexceptions -Wcast-qual -Wctor-dtor-privacy -Wempty-body -Wformat-security				\
				-Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers			\
				-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe					\
				-Winline -Wunreachable-code -Wmissing-include-dirs

CXXFLAGS	=

GCC_FLAGS			:=
GCC_DEBUG_FLAGS		:= -fsanitize=address -g3
CLANG_FLAGS			:= -stdlib=libc++
CLANG_DEBUG_FLAGS	:= -fsanitize=address -g
LD_DEBUG_FLAGS	    := -lasan

ifeq ($(CXX), g++)
	CXXFLAGS += $(GCC_FLAGS)
else ifeq ($(CXX), clang++)
	CXXFLAGS += $(CLANG_FLAGS)
endif

LDFLAGS		:= $(CXXFLAGS)
DEPFLAGS	 = -MMD -MP -MF $(OUT_NAME)
LDIBS		:=

SRC_DIR	:=  Sources Libs/Sources
INC_DIR	:= -IIncludes -ILibs/Includes
OBJ_DIR		:= objects
DEP_DIR		:= headers

EXEC		:= list
SOURCES		:= $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
SRC_NO_DIR	:= $(notdir $(SOURCES))
OBJECTS 	:= $(SRC_NO_DIR:%.cpp=$(OBJ_DIR)/%.o)

TMP_DIRS	:= $(OBJ_DIR) $(DEP_DIR)
IS_BUILT	:= 0 # Flag to indicate if a successful build has occurred

VPATH		:= $(subst $() $(),:,$(SRC_DIR))

##@ General
.PHONY:		build
build: $(EXEC)	## Build project
			@if [ $(IS_BUILT) -eq 1 ] ; then											\
				printf "$(CLR_CYAN)Project built successfully!$(CLR_END)\n";			\
			else																		\
				printf "$(CLR_MAGENTA)Project already have been built!$(CLR_END)\n";	\
			fi

$(EXEC): $(OBJECTS)
			@$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) $(LDIBS)
			$(eval IS_BUILT = 1)

%.o:		BASENAME = $(basename $(notdir $@))
			OUT_NAME = $(DEP_DIR)/$(BASENAME).d
$(OBJ_DIR)/%.o: %.cpp | $(TMP_DIRS)
			@echo $<
			@if $(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@ $(DEPFLAGS); then	\
				printf "Compiled $(BOLD_GREEN)"$<"$(CLR_END) successfully!\n";	\
			else																\
				printf "Compile $(CLR_RED)"$<"$(CLR_END) error!\n";				\
			fi

.PHONY: run
run:		## Run program
			./$(EXEC)

.PHONY: help
help:		## Display help
			@awk 'BEGIN {													\
					FS = ":.*##";											\
				}															\
				/^[a-zA-Z_0-9-]+:.*?##/ {									\
					printf "$(BOLD_BLUE)%-15s$(CLR_END)%s\n", $$1, $$2		\
				}															\
				/^##@/ {													\
					printf "\n$(FONT_BOLD)%s$(CLR_END)\n", substr($$0, 5)	\
				}															\
				END { printf "\n"; }'  Makefile

##@ Development

.PHONY: clean
clean:		## Remove binaries
			@if [ "$(wildcard $(TMP_DIRS))" -o -e $(EXEC) ]; then					\
				$(RM) -r $(OBJ_DIR);												\
				$(RM) -r $(DEP_DIR);												\
				$(RM) $(EXEC);														\
				printf "$(CLR_CYAN)Binaries removed!$(CLR_END)\n";					\
			else																	\
				printf "$(CLR_MAGENTA)Binaries already removed!$(CLR_END)\n";		\
			fi

.PHONY: debug
debug:		## Build debug version
debug:		GCC_FLAGS   += $(GCC_DEBUG_FLAGS)
debug:		CLANG_FLAGS += $(CLANG_DEBUG_FLAGS)
			LDFLAGS 	+= $(LD_DEBUG_FLAGS)
debug:		clean build

$(TMP_DIRS):
			@mkdir -p $@

-include $(OBJECTS:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)
