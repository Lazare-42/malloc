NAME = libmalloc.a

SRCS = 	malloc.c \
	   	nmap.c \
		cal_page_size.c \
		three_main_functions.c \
		free.c \

SRCS_DIR = ./srcs

ODIR = ./bin

OBJS = $(addprefix $(ODIR)/, $(SRCS:.c=.o))

INCLUDES = ./includes/

FLAGS = -Wextra -Werror -Wall -std=c99 -fno-stack-protector -pg -g3 -fsanitize=address

COMPILER = $$(command -v gcc)

CLEAN = rm -rf $(ODIR)

ARCH = ar rc

all: mkbin $(NAME)

mkbin:
	@mkdir -p $(ODIR)

$(NAME): $(OBJS) $(SELF)
	@ echo "\033[36;40mCreating malloc library:\t\t" $(NAME) "\033[0m"
	@ $(ARCH) $(NAME) $(OBJS)
	@ echo "\033[37;40mCompiled malloc library with the rules:\t" $(FLAGS) "\033[0m"

$(ODIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDES) $(SELF) Makefile
	@ echo "\033[34;40mAssembling library object file:\t\t" $@ "\033[0m"
	@ $(COMPILER) $(FLAGS) -c -o $@ $< -I$(INCLUDES)

clean: 
	@ echo "[31m Deleted bin folder. [0m"
	@-$(CLEAN)

fclean: clean
	@-$(CLEAN) $(NAME)
	@ echo "[31m Also deleted libft.a[0m"

re: fclean all
