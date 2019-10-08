ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

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

LDFLAGS = -L$(LIBDIR) -lft

COMPILER = $$(command -v gcc)

CLEAN = rm -rf $(ODIR)

ARCH = ar rc

all: mkbin $(NAME)

mkbin:
	@mkdir -p $(ODIR)

$(NAME): $(OBJS) $(SELF)
	@ echo "\033[36;40mCreating libft library:\t\t\t libft.a\033[0m"
	@ make -C libft
	@ echo "\033[36;40mCreating malloc library:\t\t" $(NAME) "\033[0m"
	@ $(COMPILER) $(FLAGS) -shared -o $@ $(OBJS) -L$(LIBDIR) -lft
	@ln -sf $(NAME) libft_malloc.so
	@ echo "\033[37;40mCompiled malloc library with the rules:\t" $(FLAGS) "\033[0m"

$(ODIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDES) $(SELF) Makefile ./libft/libft.a
	@ echo "\033[34;40mAssembling library object file:\t\t" $@ "\033[0m"
	@ $(COMPILER) $(FLAGS) -c -o  $@ $< -I$(INCLUDES) 

clean: 
	@ echo "[31m Deleted bin folder. [0m"
	@-$(CLEAN)

fclean: clean
	@-$(CLEAN) $(NAME)
	@ echo "[31m Also deleted libft.a[0m"

re: fclean all
