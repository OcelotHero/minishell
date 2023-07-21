############################### Files and directories ###############################
# Common source files
SRC_BIN = cd echo env exit export pwd unset
SRC_PAR	= ast parser interpreter get_cmd_path get_heredoc
SRC_LEX = interpolator preprocessor tokenizer
SRC_UTL =

# Mandatory source files
SRC_MAN = minishell

# Bonus source files
SRC_BNS = minishell_bonus

# Directories
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj

# Subdirectories
BLTIN_D = builtins
PARSR_D = parser
LEXER_D = lexer
UTILS_D = utils

# libft
LIBFT_D = libft
LIBFT_N = ft
LIBFT_L	= $(addprefix $(LIB_DIR)/${LIBFT_D}/lib, $(addsuffix .a, $(LIBFT_N)))

##############################        Objects        ################################
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_BIN)))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_PAR)))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_LEX)))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_UTL)))

OBJS_M	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_MAN)))

OBJS_B	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_BNS)))

##############################  Config declarations  #################################
NAME_M	= minishell
NAME_B	= minishell_bonus
INCL	= inc
OPTS	= -lreadline

VPATH	+= %.c $(SRC_DIR)
VPATH	+= %.c $(addprefix $(SRC_DIR)/, $(BLTIN_D) $(PARSR_D) $(LEXER_D) \
				 $(UTILS_D))

CC		= cc
FLAGS	= -g -O0 # -Wall -Wextra -Werror
RM		= rm -rf

all:		BNS = 0
all:		${NAME_M}

${NAME_M}:	${LIBFT_L} ${OBJS} ${OBJS_M}
			@${RM} ${OBJS_B}
			@echo "    ${NAME_M}"
			@${CC} ${FLAGS} ${OBJS} ${OBJS_M} ${LIBFT_L} -o ${NAME_M} ${OPTS}

${OBJ_DIR}/%.o: %.c | ${OBJ_DIR}
			@echo "    $<"
			@${CC} ${FLAGS} -c $< -o $@ -D BONUS=${BNS} -I ${INCL} \
				-I ${LIB_DIR}/${LIBFT_D}/${INCL}

${OBJ_DIR}:
			@mkdir -p ${OBJ_DIR}

${LIBFT_L}:
			@echo "    $@"
			@make -C ${LIB_DIR}/${LIBFT_D} bonus

clean:
			@make -C ${LIB_DIR}/${LIBFT_D} clean
			${RM} ${OBJ_DIR}

fclean:		clean
			@make -C ${LIB_DIR}/${LIBFT_D} fclean
			${RM} ${NAME_M} ${NAME_B}

bonus:		BNS = 1
bonus:		${NAME_B}

${NAME_B}:	${LIBFT_L} ${OBJS} ${OBJS_B}
			@${RM} ${OBJS_M}
			@echo "    ${NAME_B}"
			@${CC} ${FLAGS} ${OBJS} ${OBJS_M} ${LIBFT_L} -o ${NAME_M} ${OPTS}

re:			fclean all

.PHONY:		all clean fclean re