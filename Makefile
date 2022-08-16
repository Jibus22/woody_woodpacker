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

PAYLOAD = $(SRCPATH)/payload.s
##### COMPILER #####
CC = clang
##### COMPILATION FLAG #####
CCFLAGS = -Wall -Wextra -Werror

##### SRCS #####
PAYLOADSRCS = $(addprefix $(SRCPATH)/, payload.s)
NASMSRCS = $(addprefix $(SRCPATH)/, encrypt.s)
SRCS = $(addprefix $(SRCPATH)/, error.c \
			 get_elf_data.c \
			 injection_x64.c \
			 start.c \
			 utils.c)

PAYLOADOBJ = $(PAYLOADSRCS:$(SRCPATH)/%.s=$(OBJPATH)/%.o)
NASMOBJ = $(NASMSRCS:$(SRCPATH)/%.s=$(OBJPATH)/%.o)
OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)


### RULES ###

all: $(NAME)

$(NAME): $(LFT) $(OBJPATH) $(PAYLOADOBJ) $(NASMOBJ) $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ) $(NASMOBJ) $(LFT)

$(LFT):
	@make -C $(LIBFTPATH)

$(OBJPATH):
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi

$(PAYLOADOBJ): $(PAYLOADSRCS)
	nasm -f bin -o $@ $<

$(NASMOBJ): $(NASMSRCS)
	nasm -f elf64 -o $@ $<

$(OBJPATH)/%.o: $(SRCPATH)/%.c $(PAYLOADSRCS) $(HEADERS)
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@ -DPAYLOAD=\"`/usr/bin/hexdump -v -e '"\\\x" 1/1 "%02x"' $(OBJPATH)/payload.o`\"

### CLEAN ###
.PHONY: sanitize clean fclean re

clean:
	@make clean -C $(LIBFTPATH)
	rm -f woody
	rm -rf $(OBJ) $(NASMOBJ) $(PAYLOADOBJ)

fclean: clean
	@make fclean -C $(LIBFTPATH)
	rm -f $(NAME)

re: fclean
	@$(MAKE)
