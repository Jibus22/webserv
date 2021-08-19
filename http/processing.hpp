#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include "Resquest.hpp"
#include "Response.hpp"

Response const * process_request(std::string const & raw_request,
									Config_struct const & config);

#endif
