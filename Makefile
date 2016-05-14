CC		=	gcc

RM		=	rm -rf

NAME	= 	server

OBJ_DIR	=	obj/

SRC_DIR	=	src/

SRC		=	$(SRC_DIR)cmd.c		         \
            $(SRC_DIR)array.c	         \
            $(SRC_DIR)common.c	         \
            $(SRC_DIR)socket.c	         \
            $(SRC_DIR)init.c             \
            $(SRC_DIR)server.c           \
            $(SRC_DIR)manage_client.c    \
            $(SRC_DIR)ftp_cmd.c          \
            $(SRC_DIR)ftp_cmd_wd.c       \
            $(SRC_DIR)ftp_cmd_user.c     \
            $(SRC_DIR)ftp_cmd_mode.c     \
            $(SRC_DIR)ftp_cmd_file.c     \
            $(SRC_DIR)ftp_cmd_data.c     \

CFLAGS			=   -I./inc -W -Wall -Wextra -Werror -D_GNU_SOURCE -std=c99

LD_FLAGS        =   -lcrypt

OBJ		        =   $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

$(OBJ_DIR)%.o	:	$(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@echo -e "Compiling $< to $@"
	@$(CC) -c $(CFLAGS) $< -o $@

$(NAME)	:	$(OBJ)
	@echo -e "\033[32mLinking $@\033[00m"
	@$(CC) $(OBJ) -o $(NAME) $(LD_FLAGS)
	@echo -e "\033[33m${SERVER} Compiled\033[00m"

all 	: $(NAME)

clean	:
	@echo -e "\033[31mRemoving Objects\033[00m"
	@$(RM) $(OBJ)

fclean	:	clean
	@echo -e "\033[31mRemoving ${CLIENT}\033[00m"
	@echo -e "\033[31mRemoving ${SERVER}\033[00m"
	@$(RM) $(NAME)

re		:	fclean all

.PHONY	: all clean fclean re
