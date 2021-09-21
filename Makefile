NAME = webserv.out

##### SRC & OBJ PATH #####
SRCPATH = ./core
SRCPATH2 = ./conf
SRCPATH3 = ./http
SRCPATH4 = ./utils
SRCPATH5 = ./cgi

OBJPATH = $(SRCPATH)/obj
OBJPATH2 = $(SRCPATH2)/obj
OBJPATH3 = $(SRCPATH3)/obj
OBJPATH4 = $(SRCPATH4)/obj
OBJPATH5 = $(SRCPATH5)/obj

##### LIB #####
LIBS =
LIBSD = -lbsd

##### INCLUDE #####
PATH_INCLUDE = ./includes
PATH_INCLUDE1 = $(SRCPATH)/includes
PATH_INCLUDE2 = $(SRCPATH2)/includes
PATH_INCLUDE3 = $(SRCPATH3)/includes
PATH_INCLUDE4 = $(SRCPATH4)/includes
PATH_INCLUDE5 = $(SRCPATH5)/includes

HEADERS = $(PATH_INCLUDE)/*.hpp
HEADERS += $(PATH_INCLUDE1)/*.hpp
HEADERS += $(PATH_INCLUDE2)/*.hpp
HEADERS += $(PATH_INCLUDE3)/*.hpp
HEADERS += $(PATH_INCLUDE4)/*.hpp
HEADERS += $(PATH_INCLUDE5)/*.hpp

INC = $(addprefix -I , $(PATH_INCLUDE) $(PATH_INCLUDE1) $(PATH_INCLUDE2) \
	  $(PATH_INCLUDE3) $(PATH_INCLUDE4) $(PATH_INCLUDE5))


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
SRCS = $(addprefix $(SRCPATH)/, main.cpp network_endpoint.cpp Client.cpp \
	   server_process.cpp is_valid_request.cpp)
#CONF
SRCS2 = $(addprefix $(SRCPATH2)/, Config_base.cpp Config_struct.cpp \
		Location_config.cpp Server_config.cpp)
#HTTP
SRCS3 = $(addprefix $(SRCPATH3)/, Request.cpp Response.cpp processing.cpp \
		processing_utils.cpp directory_listing.cpp)
#UTILS
SRCS4 = $(addprefix $(SRCPATH4)/, close_fd.cpp errors.cpp simulations.cpp \
		utils.cpp find_nocase_header.cpp display.cpp)
#CGI
SRCS5 = $(addprefix $(SRCPATH5)/, CgiEnv.cpp process_cgi.cpp FtPipe.cpp \
		cgi_output.cpp)


##### OS CONDITIONNAL SRCS #####
ifeq ($(UNAME), Darwin)
	SRCS += $(addprefix $(SRCPATH)/, server_run_osx.cpp kqueue_event_osx.cpp)
else
	SRCS += $(addprefix $(SRCPATH)/, server_run_linux.cpp)
endif


##### OBJS #####
OBJ = $(SRCS:$(SRCPATH)/%.cpp=$(OBJPATH)/%.o)
OBJ += $(SRCS2:$(SRCPATH2)/%.cpp=$(OBJPATH2)/%.o)
OBJ += $(SRCS3:$(SRCPATH3)/%.cpp=$(OBJPATH3)/%.o)
OBJ += $(SRCS4:$(SRCPATH4)/%.cpp=$(OBJPATH4)/%.o)
OBJ += $(SRCS5:$(SRCPATH5)/%.cpp=$(OBJPATH5)/%.o)


### RULES ###

all : mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi
	@if [ ! -d $(OBJPATH2) ]; then mkdir $(OBJPATH2); fi
	@if [ ! -d $(OBJPATH3) ]; then mkdir $(OBJPATH3); fi
	@if [ ! -d $(OBJPATH4) ]; then mkdir $(OBJPATH4); fi
	@if [ ! -d $(OBJPATH5) ]; then mkdir $(OBJPATH5); fi


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

$(OBJPATH3)/%.o : $(SRCPATH3)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(OBJPATH4)/%.o : $(SRCPATH4)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(OBJPATH5)/%.o : $(SRCPATH5)/%.cpp $(HEADERS)
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
