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
CCFLAGS =

##### SRCS #####
SRCS = $(addprefix $(SRCPATH)/, start.c error.c injection_x64.c)

OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)


### RULES ###

all: mk_objdir $(NAME)


mk_objdir:
	@rm -f woody
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi


$(NAME): $(LFT) $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ) $(LFT)

$(LFT):
	@make -C $(LIBFTPATH)

$(OBJPATH)/%.o: $(SRCPATH)/%.c $(HEADERS)
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@

### CLEAN ###
.PHONY: sanitize clean fclean re

clean:
	@make clean -C $(LIBFTPATH)
	rm -f woody
	rm -rf $(OBJ)

fclean: clean
	@make fclean -C $(LIBFTPATH)
	rm -f $(NAME)

re: fclean
	@$(MAKE)
