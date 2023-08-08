############################### Files and directories ###############################
# Common source files
SRC_BIN = cd echo env exit export pwd unset
SRC_PAR	= ast parser get_cmd_path get_heredoc
SRC_LEX = interpolator preprocessor postprocessor
SRC_SGN = handler
SRC_UTL = utils auxilary
SRC_ENT = minishell
SRC_INT = interpreter executables redirector

# Bonus source files
SRC_BNS = glob

# Conditional recompilation source files
SRC_REC = trimmer tokenizer

# Directories
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj

# Subdirectories
BLTIN_D = builtins
INTER_D = interpreter
PARSR_D = parser
LEXER_D = lexer
SIGNL_D = signals
UTILS_D = utils


# libft
LIBFT_D = libft
LIBFT_N = ft
LIBFT_L	= $(addprefix $(LIB_DIR)/${LIBFT_D}/lib, $(addsuffix .a, $(LIBFT_N)))

# ft_dprintf
FPRNF_D = ft_dprintf
FPRNF_N = ftdprintf
FPRNF_L	= $(addprefix $(LIB_DIR)/${FPRNF_D}/lib, $(addsuffix .a, $(FPRNF_N)))

##############################        Objects        ################################
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_BIN)))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_PAR)))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_INT)))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_LEX)))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_SGN)))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_UTL)))

OBJS	+= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_ENT)))

OBJS_R	= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_REC)))

OBJS_B	= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_BNS)))

##############################  Config declarations  #################################
NAME	= minishell
INCL	= inc
OPTS	= -lreadline

VPATH	+= %.c $(SRC_DIR)
VPATH	+= %.c $(addprefix $(SRC_DIR)/, $(BLTIN_D) $(PARSR_D) $(INTER_D) \
			$(LEXER_D) $(SIGNL_D) $(UTILS_D))

CC		= cc
FLAGS	= -g -O0 -MD # -Wall -Wextra -Werror
RM		= rm -rf

OSNAME	= $(shell uname -s)

ifeq ($(shell test -d "/Users/${USER}/.brew"; echo $$?), 1)
	BREW_D = homebrew
else
	BREW_D = .brew
endif

ifeq (${OSNAME}, Darwin)
	OPTS	+= "-L/Users/${USER}/${BREW_D}/opt/readline/lib"
	INCL	+= "-I/Users/${USER}/${BREW_D}/opt/readline/include"
endif

all:		${NAME}

${NAME}:	BNS = 0
${NAME}:	clrbns ${LIBFT_L} ${FPRNF_L} ${OBJS} ${OBJS_R}
			@${CC} ${FLAGS} ${OBJS} ${OBJS_R} ${LIBFT_L} ${FPRNF_L} -o ${NAME} ${OPTS}

clrbns:
			@if [ -f ${OBJS_B} ]; then ${RM} ${OBJS_B} ${OBJS_R} ${NAME}; fi

${OBJ_DIR}/%.o: %.c | ${OBJ_DIR}
			@echo "    $<"
			@${CC} ${FLAGS} -c $< -o $@ -D BONUS=${BNS} -I ${INCL} \
				-I ${LIB_DIR}/${LIBFT_D}/${INCL} -I ${LIB_DIR}/${FPRNF_D}/${INCL}

${OBJ_DIR}:
			@mkdir -p ${OBJ_DIR}

${LIBFT_L}:
			@echo "    $@"
			@make -C ${LIB_DIR}/${LIBFT_D} bonus

${FPRNF_L}:
			@echo "    $@"
			@make -C ${LIB_DIR}/${FPRNF_D} bonus

clean:
			@make -C ${LIB_DIR}/${LIBFT_D} clean
			@make -C ${LIB_DIR}/${FPRNF_D} clean
			${RM} ${OBJ_DIR}

fclean:		clean
			@make -C ${LIB_DIR}/${LIBFT_D} fclean
			@make -C ${LIB_DIR}/${FPRNF_D} fclean
			${RM} ${NAME_M} ${NAME_B}

bonus:		BNS = 1
bonus:		clrman ${LIBFT_L} ${FPRNF_L} ${OBJS} ${OBJS_R} ${OBJS_B}
			@${CC} ${FLAGS} ${OBJS} ${OBJS_R} ${OBJS_B} ${LIBFT_L} ${FPRNF_L} \
				-o ${NAME} ${OPTS}

clrman:
			@if [ ! -f ${OBJS_B} ]; then ${RM} ${OBJS_R} ${NAME}; fi

re:			fclean all

.PHONY:		all clean fclean re bonus
