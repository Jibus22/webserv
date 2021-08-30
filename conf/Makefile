# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frfrance <frfrance@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/09 19:14:21 by frfrance          #+#    #+#              #
#    Updated: 2021/08/26 19:03:08 by frfrance         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = a.out
OBJSDIR = .objs
SRCS = main.cpp \
		Config_base.cpp \
		Server_config.cpp \
		Location_config.cpp \
		Config_struct.cpp

OBJS = $(addprefix $(OBJSDIR)/, $(SRCS:.cpp=.o))
DPDCS = $(OBJS:.o=.d)
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = clang++
opti :
		@(make -j all)

all : $(NAME)

-include $(DPDCS)

$(NAME) : $(OBJS)
		@(clang++ $(CXXFLAGS) $(OBJS) -o $(NAME))
		@(echo "$(NAME) created")

$(OBJSDIR)/%.o : %.cpp | $(OBJSDIR)
		@(echo "Compiling -> $^")
		@(clang++ $(CXXFLAGS) -MMD -c $< -o $@)

$(OBJSDIR) :
		@(mkdir -p .objs)

clean :
		@(rm -f $(NAME))

fclean : clean
		@(rm -rf $(OBJSDIR))

re : fclean all
