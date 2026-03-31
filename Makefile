CXX = c++
CC = cc
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -g
GLADFLAGS = -I glad/include
OPENGLFLAGS = -lGL -lglfw
NAME = scop
SRC = src/main.cpp src/shader.cpp src/texture.cpp
GLAD_SRC = glad/src/glad.c
GLAD_OBJ = glad/src/glad.o
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ) $(GLAD_OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(GLAD_OBJ) -o $(NAME) $(OPENGLFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(GLADFLAGS) -c $< -o $@

$(GLAD_OBJ): $(GLAD_SRC)
	$(CC) $(GLADFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(GLAD_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re