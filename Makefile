CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -ggdb
INC = -Iinc/classes -Iinc
BUILD = build
OUT = server

HEADERSFILES_ = $(wildcard inc/*.hpp) $(wildcard inc/**/*.hpp)

HEADERSFILES = $(subst inc/, , $(HEADERSFILES_:.hpp=)) 
				
HEADERS := $(addprefix inc/, $(HEADERSFILES:=.hpp))

FILES_ = $(wildcard *.cpp) $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)

FILES__ := $(subst src/, , $(FILES_))

FILES = $(FILES__:%.cpp=%)

OBJS := $(addprefix $(BUILD)/, $(FILES:=.o))

all : $(OUT)
	
$(OUT) : $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(OUT)

$(BUILD)/%.o : src/%.cpp $(HEADERS)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INC)

clean :
	rm -rf $(BUILD)

fclean : clean
	rm -rf $(OUT)

re : fclean all

.PHONY : re fclean clean all