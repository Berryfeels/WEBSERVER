CXX := c++
CXXFLAGS := #-Werror -Wextra -Wall

OBJ_DIR := obj

NAME := webserver

SRC := $(shell find . -name "*.cpp")

OBJ := $(patsubst ./%cpp, $(OBJ_DIR)/%.o, $(SRC))

all : $(NAME)

$(NAME) : $(OBJ)
			$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: ./%.cpp
			@mkdir -p $(dir $@)
			$(CXX) $(CXXFLAGS) -c $< -o $@

clean: 
		rm -rf $(OBJ_DIR)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re