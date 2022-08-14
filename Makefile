NAME = woody_woodpacker

##### SRC & OBJ PATH #####
SRCPATH = srcs
OBJPATH = $(SRCPATH)/obj

##### LIB #####
LIBFTPATH = ./libft
LFT = -L $(LIBFTPATH) -lft

##### INCLUDE #####
PATH_INCLUDE = $(SRCPATH)/includes
PATH_INCLUDE2 = libft
HEADERS = $(PATH_INCLUDE)/*.h
HEADERS += $(PATH_INCLUDE2)/*.h
INC = $(addprefix -I , $(PATH_INCLUDE) $(PATH_INCLUDE2))


##### COMPILER #####
CC = clang
##### COMPILATION FLAG #####
CCFLAGS = -Wall -Wextra -Werror

##### SRCS #####
SRCS = $(addprefix $(SRCPATH)/, start.c error.c injection_x64.c \
			 get_elf_data.c utils.c)
NASMSRCS = $(addprefix $(SRCPATH)/, payload.s)

OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)
NASMOBJ = $(NASMSRCS:$(SRCPATH)/%.s=$(OBJPATH)/%.o)


### RULES ###

all: $(NAME)

$(NAME): $(LFT) $(OBJPATH) $(NASMOBJ) $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ) $(LFT)

$(OBJPATH):
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi

$(LFT):
	@make -C $(LIBFTPATH)

$(OBJPATH)/%.o: $(SRCPATH)/%.c srcs/payload.s $(HEADERS)
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@ -DPAYLOAD=\"`/usr/bin/hexdump -v -e '"\\\x" 1/1 "%02x"' $(OBJPATH)/payload.o`\"

$(OBJPATH)/%.o: $(SRCPATH)/%.s
	nasm -f bin -o $@ $<

### CLEAN ###
.PHONY: sanitize clean fclean re

clean:
	@make clean -C $(LIBFTPATH)
	rm -f woody
	rm -rf $(OBJ) $(NASMOBJ)

fclean: clean
	@make fclean -C $(LIBFTPATH)
	rm -f $(NAME)

re: fclean
	@$(MAKE)
