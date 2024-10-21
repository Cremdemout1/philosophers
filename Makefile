# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#              #
#    Updated: 2024/10/21 16:06:59 by ycantin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers
CC = cc
TSAN_FLAGS = -fsanitize=thread
FLAGS = -Wall -Wextra -Werror -g 
RM = rm -f
DEPENDENCIES = -pthread

SRC  = 	main.c \
		create_philosophers.c \
		threads.c \
		threads_utils.c \
		mutex_safety.c

GREEN = \033[1;32m
YELLOW = \033[1;33m
CYAN = \033[1;36m
BLUE = \033[1;34m
RED = \033[1;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(LIBFT) $(SRC)
	@$(CC) $(FLAGS) $(SRC) -o $(NAME) $(DEPENDENCIES)
	@echo $(NAME) ready
	@echo "$(CYAN)Usage:$(RESET) ./$(NAME) $(GREEN)<num_philosophers>$(RESET) $(YELLOW)<time_to_die>$(RESET) $(MAGENTA)<time_to_eat>$(RESET) $(BLUE)<time_to_sleep>$(RESET) $(RED)[num_meals]$(RESET)"
	@echo "$(GREEN)- <num_philosophers>: Number of philosophers at the table.$(RESET)"
	@echo "$(YELLOW)- <time_to_die>: Time (ms) before a philosopher dies if not eating.$(RESET)"
	@echo "$(MAGENTA)- <time_to_eat>: Time (ms) it takes for a philosopher to eat.$(RESET)"
	@echo "$(BLUE)- <time_to_sleep>: Time (ms) a philosopher spends sleeping.$(RESET)"
	@echo "$(RED)- [num_meals]: (Optional) Number of meals before simulation ends.$(RESET)"
	@echo "$(CYAN)Example:$(RESET) ./$(NAME) $(GREEN)5$(RESET) $(YELLOW)800$(RESET) $(MAGENTA)200$(RESET) $(BLUE)200$(RESET) $(RED)3$(RESET)"
	@echo "$(CYAN)This runs a simulation with 5 philosophers, 800ms to die, 200ms to eat, 200ms to sleep, and 3 meals to finish.$(RESET)"

clean: 
	@echo $(NAME) clean
	
fclean: clean
	@$(RM) $(NAME)
	@echo $(NAME) fully cleaned
	
re: fclean all