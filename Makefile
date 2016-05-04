CC		=	gcc -g

RM		=	rm -rf

NAME	= 	my_ftp

CLIENT	=	client

SERVER	=	server

OBJ_DIR	=	obj/

SRC_DIR	=	src/

SRC_DIR_CLIENT	=	$(SRC_DIR)client/

SRC_DIR_SERVER	=	$(SRC_DIR)server/

SRC				=	$(SRC_DIR)cmd.c		\
					$(SRC_DIR)array.c	\
					$(SRC_DIR)common.c	\
					$(SRC_DIR)socket.c	\

SRC_CLIENT		=	$(SRC_DIR_CLIENT)client.c       \
                    $(SRC_DIR_CLIENT)cmd_manage.c   \

SRC_SERVER		=	$(SRC_DIR_SERVER)server.c           \
                    $(SRC_DIR_SERVER)manage_client.c    \
                    $(SRC_DIR_SERVER)ftp_cmd.c          \
                    $(SRC_DIR_SERVER)ftp_cmd_wd.c       \
                    $(SRC_DIR_SERVER)ftp_cmd_user.c     \
                    $(SRC_DIR_SERVER)ftp_cmd_mode.c     \
                    $(SRC_DIR_SERVER)ftp_cmd_file.c     \

SRC_CLIENT		+=	$(SRC)

SRC_SERVER		+=	$(SRC)

CFLAGS			=   -I./inc -W -Wall -Wextra -Werror

LD_FLAGS        =   -lcrypt

OBJ_CLIENT		= $(SRC_CLIENT:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

OBJ_SERVER		= $(SRC_SERVER:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

$(OBJ_DIR)%.o	:	$(SRC_DIR)%.c
	@mkdir -p obj/client
	@mkdir -p obj/server
	@echo -e "Compiling $< to $@"
	@$(CC) -c $(CFLAGS) $< -o $@

$(NAME)	: 	$(SERVER) ##$(CLIENT)

all 	:	$(NAME)

$(CLIENT)	:	$(OBJ_CLIENT)
	@echo -e "\033[32mLinking $@\033[00m"
	@$(CC) $(OBJ_CLIENT) -o $(CLIENT)
	@echo -e "\033[33m${CLIENT} Compiled\033[00m"

$(SERVER)	:	$(OBJ_SERVER)
	@echo -e "\033[32mLinking $@\033[00m"
	@$(CC) $(OBJ_SERVER) -o $(SERVER) $(LD_FLAGS)
	@echo -e "\033[33m${SERVER} Compiled\033[00m"

clean	:
	@echo -e "\033[31mRemoving Objects\033[00m"
	@$(RM) $(OBJ)
	@$(RM) $(OBJ_CLIENT)
	@$(RM) $(OBJ_SERVER)

fclean	:	clean
	@echo -e "\033[31mRemoving ${CLIENT}\033[00m"
	@echo -e "\033[31mRemoving ${SERVER}\033[00m"
	@$(RM) $(CLIENT) $(SERVER)

re		:	fclean all

.PHONY	: all clean fclean $(CLIENT) $(SERVER) re
