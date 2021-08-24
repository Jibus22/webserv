

# include "includes/Location_config.hpp"

Location_config::Location_config(){
  this->auto_index = false;
}

std::ostream& operator<<(std::ostream& os, Location_config &ref){
  os << "INDEX = " << ref.index << std::endl;
  os << "ROOT = " << ref.root << std::endl;

  for (Location_config::c_methode_vector::iterator it = ref.methode.begin(); it != ref.methode.end(); it++)
    os << "ALLOW_REQUEST = " << *it << std::endl;

  os << "AUTO_INDEX = " << ref.auto_index << "\n";

  for (Location_config::c_cgi_map::iterator it = ref.cgi.begin(); it != ref.cgi.end(); it++)
	  os << "CGI_EXT exec = " << it->first << "\nCGI_EXT chemin = " << it->second << std::endl;
  
  for (Location_config::c_error_map::iterator it = ref.error_page.begin(); it != ref.error_page.end(); it++)
	  os << "ERROR_PAGE NUMBER = " << it->first << "\nERROR_PAGE = " << it->second << std::endl;
  return (os);
}

