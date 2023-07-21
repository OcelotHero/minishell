############################### Files and directories ###############################
# Mandatory source files
SRC_ELE = atoi bzero calloc isalnum isalpha isascii isdigit isprint memchr memcmp \
		  memcpy memmove memset strchr strdup strlcat strlcpy strlen strcmp strncmp \
		  strnstr strrchr tolower toupper swap
SRC_SUP = itoa putchar_fd putendl_fd putnbr_fd putstr_fd split striteri strjoin \
		  strmapi strtrim substr

# Bonus source files
SRC_BNS = lstadd_back lstadd_front lstclear lstdelone lstiter lstlast lstmap lstnew \
		  lstsize lstremove_if

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Subdirectories
ELEMN_D = elementary
SUPPL_D = supplementary
BONUS_D = bonus

##############################        Objects        ################################
OBJS_M	+= $(addprefix $(OBJ_DIR)/ft_, $(addsuffix .o, $(SRC_ELE)))
OBJS_M	+= $(addprefix $(OBJ_DIR)/ft_, $(addsuffix .o, $(SRC_SUP)))

OBJS_B	+= $(addprefix $(OBJ_DIR)/ft_, $(addsuffix .o, $(SRC_BNS)))

##############################  Config declarations  #################################
NAME	= libft.a
INCL	= inc

VPATH	+= %.c $(addprefix $(SRC_DIR)/, $(ELEMN_D) $(SUPPL_D) $(BONUS_D))

LIB		= ar rc
CC		= cc
FLAGS	= -g -Wall -Wextra -Werror
RM		= rm -rf


all:		${NAME}

${NAME}:	${OBJS_M}
			@echo "    ${NAME}"
			@${LIB} ${NAME} ${OBJS_M}

${OBJ_DIR}/%.o: %.c | ${OBJ_DIR}
			@echo "    $<"
			@${CC} ${FLAGS} -c $< -o $@ -I ${INCL}

${OBJ_DIR}:
			@mkdir -p ${OBJ_DIR}

clean:
			${RM} ${OBJ_DIR}

fclean:		clean
			${RM} ${NAME}

bonus:		${OBJS_M} ${OBJS_B}
			@echo "    ${NAME}"
			@${LIB} ${NAME} ${OBJS_M} ${OBJS_B}

re:			fclean all

.PHONY:		all clean fclean re bonus