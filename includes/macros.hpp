#ifndef MACROS_HPP
#define MACROS_HPP

#include <sstream>

//__________________________________MACROS____________________________________//
#ifndef _DEBUG
# define _DEBUG 0
#endif

# define __D_DISPLAY(fmt) \
	if (_DEBUG) {\
	std::ostringstream ost;\
	ost << fmt << std::endl;\
	std::cout << ost.str();} else ;\

# define RCV_BUF 5
# define MAX_EVENTS 32

# define EMPTY 2
# define READY 1

# define VALID_REQUEST 0
# define INCOMPLETE_REQUEST 1
# define CORRUPT_REQUEST 2

# define CGI_RD_BUF_LEN 16
# define CGI_TIME_LIMIT 500//in ms.

# define CGI_ERR 2
# define CGI_REDIRECT 1
# define CGI_SUCCESS 0

#endif
