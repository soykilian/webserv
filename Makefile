NAME	= webserv

CC		= c++

CFLAGS	= -Werror -Wextra -Wall -std=c++98 -pedantic -fsanitize=address

SRCS_MAIN	= main.cpp

SRCS_CONFIG	= Config.cpp

SRCS		= ${SRCS_MAIN} \
				$(addprefix config/, ${SRCS_CONFIG})

OBJS	= ${SRCS:.cpp=.o}

SRCS_DIR = src

OBJS_DIR = obj

OBJS_DIRS	= ${OBJS_DIR} \
				$(addprefix ${OBJS_DIR}/, config)

SRCS_PATHS 	= $(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS_PATHS 	= $(addprefix $(OBJS_DIR)/, $(OBJS))

INCLUDES = -I ./inc -I ./src/config

RM		= rm -rf

all: ${NAME}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

$(NAME): ${OBJS_DIRS} ${OBJS_PATHS} 
	${CC} ${CFLAGS} ${INCLUDE} ${OBJS_PATHS} -o ${NAME}

$(OBJS_DIRS):
	@mkdir -p $(OBJS_DIRS) 2> /dev/null

clean:
	@${RM} *.a ${OBJS_DIRS}/*.o *.dSYM
	@${RM} ${OBJS_DIRS}

fclean:		clean
	@${RM} ${NAME}

bonus: all

re:		fclean all

.PHONY:	clean re fclean all
