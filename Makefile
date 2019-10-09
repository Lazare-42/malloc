ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc.so

HOSTLIB = libft_malloc_$(HOSTTYPE).so

SRCS = 	malloc.c \
	   	nmap.c \
		cal_page_size.c \
		three_main_functions.c \
		free.c \

SRCS_DIR = ./srcs

ODIR = ./bin

LIBDIR = libft

OBJS = $(addprefix $(ODIR)/, $(SRCS:.c=.o))

INCLUDES = ./includes/

FLAGS = -Wextra -Werror -Wall -std=c99 -fno-stack-protector -fsanitize=address 

LDFLAGS = -shared

COMPILER = $$(command -v gcc)

CLEAN = rm -rf $(ODIR)

LIBFT = libft

all: mkbin $(NAME)

mkbin:
	@mkdir -p $(ODIR)

$(LIBDIR): 
	@[[ -d libft ]] || (echo Cloning [libft]... && git clone https://github.com/Lazare-42/libft &>/dev/null)
	@make -C libft/

$(NAME): $(LIBDIR) $(OBJS) $(SELF)
	@ $(COMPILER) $(FLAGS) $(LDFLAGS) -o $@ $(OBJS) -L$(LIBDIR) -lft
	@ln -sf $(HOSTLIB) $(NAME)
	@echo "\033[37;40mCompiled malloc library with the rules:\t" $(FLAGS) "\033[0m"

$(ODIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDES) $(SELF) Makefile ./libft/libft.a 
	@ echo "\033[34;40mAssembling library object file:\t\t" $@ "\033[0m"
	@ $(COMPILER) $(FLAGS) -c -o  $@ $< -I$(INCLUDES) 

clean: 
	@ make -C libft/ clean 
	@ echo "[31m Deleted bin folder. [0m"
	@-$(CLEAN)

fclean: clean
	@-$(CLEAN) $(NAME)
	@ make -C libft/ fclean 

re: fclean all
