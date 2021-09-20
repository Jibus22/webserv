

# include "includes/Location_config.hpp"

Location_config::Location_config(){
  this->auto_index = false;
}

std::ostream& operator<<(std::ostream& os, Location_config &ref){
  
  os << "URI = {" << ref.uri << "}" << std::endl;

  for (Location_config::c_index_vector::iterator it = ref.index.begin(); it != ref.index.end(); it++)
    os << "INDEX = " << "{" << *it << "}" << std::endl;

  os << "ROOT = " << "{" << ref.root << "}" << std::endl;

  for (Location_config::c_methode_vector::iterator it = ref.methode.begin(); it != ref.methode.end(); it++)
    os << "ALLOW_REQUEST = " << "{" << *it << "}" << std::endl;

  if (ref.auto_index == true)
    std::cout << "AUTO_INDEX = ON " "\n";
  else
    std::cout << "AUTO_INDEX = OFF " "\n";
 
  for (Location_config::c_cgi_map::iterator it = ref.cgi.begin(); it != ref.cgi.end(); it++)
	  os << "CGI_EXT exec = "  << "{" << it->first << "}" << "\nCGI_EXT chemin = " << "{" << it->second  << "}" << std::endl;

  os << "UPLOAD_DIR = " <<  "{" << ref.upload_dir << "}" << "\n";

  os << "ERROR NUMBER = " << "{" << ref.return_p.first << "}" << "\n" << "ERROR URL = " << "{" << ref.return_p.second << "}" << "\n";
  
  return (os);
}

