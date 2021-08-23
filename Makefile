NAME = webserv.out

##### SRC & OBJ PATH #####
SRCPATH = ./core
SRCPATH2 = ./conf
#SRCPATH3 = ./http
SRCPATH4 = ./utils

OBJPATH = $(SRCPATH)/obj
OBJPATH2 = $(SRCPATH2)/obj
#OBJPATH3 = $(SRCPATH3)/obj
OBJPATH4 = $(SRCPATH4)/obj

##### LIB #####
LIBS =
LIBSD = -lbsd

##### INCLUDE #####
PATH_INCLUDE = ./includes
PATH_INCLUDE2 = $(SRCPATH2)/includes
HEADERS = $(PATH_INCLUDE)/*.hpp
HEADERS += $(PATH_INCLUDE2)/*.hpp
INC = $(addprefix -I , $(PATH_INCLUDE) $(PATH_INCLUDE2))


##### COMPILER #####
CXX = clang++
##### COMPILATION FLAG #####
CXXFLAGS = -Wall -Wextra -Werror -std=c++98


##### OSTYPE #####
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CXXFLAGS += -D DARWIN
else
	CXXFLAGS += -D LINUX
endif


##### SRCS #####
#CORE
SRCS = $(addprefix $(SRCPATH)/, main.cpp network_endpoint.cpp)
#CONF
SRCS2 = $(addprefix $(SRCPATH2)/, Config_base.cpp Config_struct.cpp \
		Location_config.cpp Server_config.cpp)
#HTTP
#SRCS3 = $(addprefix $(SRCPATH2)/, )
#UTILS
SRCS4 = $(addprefix $(SRCPATH4)/, close_fd.cpp errors.cpp simulations.cpp)

##### OS CONDITIONNAL SRCS #####
ifeq ($(UNAME), Darwin)
	SRCS += $(addprefix $(SRCPATH)/, server_engine.cpp)
else
	SRCS += $(addprefix $(SRCPATH)/, server_engine_linux.cpp)
endif


##### OBJS #####
OBJ = $(SRCS:$(SRCPATH)/%.cpp=$(OBJPATH)/%.o)
OBJ += $(SRCS2:$(SRCPATH2)/%.cpp=$(OBJPATH2)/%.o)
#OBJ += $(SRCS3:$(SRCPATH3)/%.cpp=$(OBJPATH3)/%.o)
OBJ += $(SRCS4:$(SRCPATH4)/%.cpp=$(OBJPATH4)/%.o)


### RULES ###

all : mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi
	@if [ ! -d $(OBJPATH2) ]; then mkdir $(OBJPATH2); fi
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

$(OBJPATH2)/%.o : $(SRCPATH2)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

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
