NAME = webserv.out

##### SRC & OBJ PATH #####
SRCPATH = ./core
#SRCPATH2 = ./http
#SRCPATH3 = ./parsing
SRCPATH4 = ./utils

OBJPATH = $(SRCPATH)/obj
#OBJPATH2 = $(SRCPATH2)/obj
#OBJPATH3 = $(SRCPATH3)/obj
OBJPATH4 = $(SRCPATH4)/obj

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
SRCS = $(addprefix $(SRCPATH)/, main.cpp network_endpoint.cpp server_engine.cpp)
#SRCS2 = $(addprefix $(SRCPATH2)/, )
#SRCS3 = $(addprefix $(SRCPATH2)/, )
SRCS4 = $(addprefix $(SRCPATH4)/, close_fd.cpp sys_err.cpp)

OBJ = $(SRCS:$(SRCPATH)/%.cpp=$(OBJPATH)/%.o)
#OBJ += $(SRCS2:$(SRCPATH2)/%.cpp=$(OBJPATH2)/%.o)
#OBJ += $(SRCS3:$(SRCPATH3)/%.cpp=$(OBJPATH3)/%.o)
OBJ += $(SRCS4:$(SRCPATH4)/%.cpp=$(OBJPATH4)/%.o)


### RULES ###

all : mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi
	@#@if [ ! -d $(OBJPATH2) ]; then mkdir $(OBJPATH2); fi
	@#@if [ ! -d $(OBJPATH3) ]; then mkdir $(OBJPATH3); fi
	@if [ ! -d $(OBJPATH4) ]; then mkdir $(OBJPATH4); fi


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

#$(OBJPATH3)/%.o : $(SRCPATH3)/%.cpp $(HEADERS)
	#$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(OBJPATH4)/%.o : $(SRCPATH4)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

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
