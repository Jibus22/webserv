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
# define INCOMPLETE 2
# define COMPLETE 1
# define READY 0


#endif
