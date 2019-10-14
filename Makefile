ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc.so

HOSTLIB = libft_malloc_$(HOSTTYPE).so

SRCS = 	malloc.c \
	   	nmap.c \
		cal_page_size.c \
		cal_page_size_2.c \
		three_main_functions.c \
		free.c \
		show_alloc_mem.c \
		init_mem_block.c \

SRCS_DIR = ./srcs

ODIR = ./bin

LIBDIR = ./libft

OBJS = $(addprefix $(ODIR)/, $(SRCS:.c=.o))

INCLUDES = ./includes/

INCLUDES_LIBFT = ./libft/includes/

FLAGS = -fPIC -Wextra -Werror -Wall -std=c99 #-fsanitize=address 

LDFLAGS = -shared -exported_symbols_list symbol_list

COMPILER = $$(command -v gcc)

CLEAN = rm -rf $(ODIR)

LIBFT = ./libft/libft.a

all: mkbin $(NAME)

mkbin:
	@mkdir -p $(ODIR)

$(NAME): $(LIBDIR) $(LIBFT) $(OBJS) $(SELF)
	@$(COMPILER) $(FLAGS) $(LDFLAGS) -o $(HOSTLIB) $(OBJS) -L$(LIBDIR) -lft
	@ln -sf $(HOSTLIB) $(NAME)
	@ echo "\033[37;40mCompiled malloc library with the rules:\t" $(FLAGS) "\033[0m"

$(LIBDIR):  
	@[[ -d libft ]] || (echo Cloning [libft]... && git clone https://github.com/Lazare-42/libft &>/dev/null)

$(LIBFT):
	@make -C libft/

$(ODIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDES) $(SELF) Makefile 
	@ echo "\033[34;40mAssembling library object file:\t\t" $@ "\033[0m"
	@ $(COMPILER) $(FLAGS) -c -o  $@ $< -I$(INCLUDES) -I$(INCLUDES_LIBFT)

clean: 
	@ make -C libft/ clean 
	@ echo "[31m Deleted bin folder. [0m"
	@-$(CLEAN)

fclean: clean
	@-$(CLEAN) $(NAME)
	@ make -C libft/ fclean 

test: $(NAME) main.c
	@$(COMPILER) $(FLAGS) main.c -o test -L. -lft_malloc
	@./run.sh ./test
re: fclean all

.PHONY: all
