NAME	= webserv

CC		= c++

CONFIG_FILE = webserv.conf

CFLAGS	= -Werror -Wextra -Wall -std=c++98 -pedantic -D CONFIG_FILE=\"$(CONFIG_FILE)\" -fsanitize=address -g3

SRCS_MAIN	= main.cpp

SRCS_CONFIG	= Config.cpp

SRCS_SERVER	= Server.cpp Request.cpp Response.cpp Cluster.cpp

SRCS_UTILS	= Utils.cpp

SRCS_FIELDS	= AllowedMethodsField.cpp Base.cpp ClientBodySizeField.cpp \
			  IndexField.cpp Location.cpp ServerNameField.cpp \
			  ErrorPageField.cpp ListenField.cpp RootField.cpp \
			  LoadFolderField.cpp RedirectionField.cpp \
				AutoindexField.cpp CGIField.cpp


SRCS		= ${SRCS_MAIN} \
				$(addprefix config/, ${SRCS_CONFIG}) \
				$(addprefix utils/, ${SRCS_UTILS}) \
				$(addprefix server/, ${SRCS_SERVER}) \
				$(addprefix config/fields/, ${SRCS_FIELDS})

OBJS	= ${SRCS:.cpp=.o}

SRCS_DIR = src

OBJS_DIR = obj

OBJS_DIRS	= ${OBJS_DIR} \
				$(addprefix ${OBJS_DIR}/, config) \
				$(addprefix ${OBJS_DIR}/, utils) \
				$(addprefix ${OBJS_DIR}/, config/fields) \
				$(addprefix ${OBJS_DIR}/, server)

SRCS_PATHS 	= $(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS_PATHS 	= $(addprefix $(OBJS_DIR)/, $(OBJS))

INCLUDES = -I ./inc -I ./src/config -I ./src/config/fields/inc -I ./src/utils -I ./src/server

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

bear : fclean
	@bear -- make

rrun: re
	./${NAME}

run: all
	./${NAME}

re:		fclean all

debug: CFLAGS += -g -D DEBUG
debug: all

rdebug: CFLAGS += -g -D DEBUG
rdebug: re

.PHONY:	clean re fclean all bear run rrun debug rdebug
