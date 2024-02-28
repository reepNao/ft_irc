NAME		= ircserv
CC			= c++ -std=c++98
FLAGS		= -Wall -Wextra -Werror

SRC			= $(wildcard src/*.cpp) $(wildcard src/Commands/*.cpp)

OBJ			= $(SRC:src/%.cpp=obj/%.o)

all			: $(NAME)

$(NAME)		: obj $(OBJ)
	$(CC) $(FLAGS) $(OBJ) main.cpp -o $(NAME)

obj			:
	mkdir -p obj
	mkdir -p obj/Commands

obj/%.o		: src/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean		:
	rm -rf log.log
	rm -rf error.log
	rm -rf $(NAME)

fclean		: clean
	rm -rf obj

re			: clean all
