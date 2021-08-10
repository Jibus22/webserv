NAME = webserv.out

##### SRC & OBJ PATH #####
SRCPATH = ./core
#SRCPATH2 = ./http
#SRCPATH3 = ./parsing
OBJPATH = $(SRCPATH)/obj
#OBJPATH2 = $(SRCPATH2)/obj
#OBJPATH3 = $(SRCPATH3)/obj

##### LIB #####
LIBS =
LIBSD = -lbsd

##### INCLUDE #####
PATH_INCLUDE = ./includes
HEADERS = $(PATH_INCLUDE)/*.hpp
INC = $(addprefix -I , $(PATH_INCLUDE))


##### COMPILER #####
CXX = clang++
##### COMPILATION FLAG #####
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

##### OSTYPE #####
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# mac
	CXXFLAGS += -D DARWIN
else
	#Linux and others...
	CXXFLAGS += -D LINUX
endif

##### SRCS #####
SRCS = $(addprefix $(SRCPATH)/, main.cpp)
#SRCS2 = $(addprefix $(SRCPATH2)/, )

OBJ = $(SRCS:$(SRCPATH)/%.cpp=$(OBJPATH)/%.o)
#OBJ += $(SRCS2:$(SRCPATH2)/%.cpp=$(OBJPATH2)/%.o)


### RULES ###

all : mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi


$(NAME) : message $(OBJ)
	@echo "\n$(END)$(BLUE)# Making $(NAME) #$(END)$(GREY)"
	$(CXX) -o $@ $(OBJ)
	@echo "\n$(END)$(GREEN)# $(NAME) is built #$(END)"

message :
	@echo "\n$(END)$(BLUE)# Making $(NAME) objects #$(END)$(GREY)"


$(OBJPATH)/%.o : $(SRCPATH)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

#$(OBJPATH2)/%.o : $(SRCPATH2)/%.cpp $(HEADERS)
	#$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

.PHONY : sanitize clean fclean re debug

debug: CXXFLAGS += -D _DEBUG
debug: all

### CLEAN ###

clean :
	@echo "$(END)$(RED)# removing $(NAME) objects #$(END)$(GREY)"
	rm -rf $(OBJ)

fclean : clean
	@echo "$(END)$(RED)\n# removing $(NAME) #$(END)$(GREY)"
	rm -f $(NAME)

re : fclean all

