

# include "includes/Location_config.hpp"

Location_config::Location_config(){

}

std::ostream& operator<<(std::ostream& os, Location_config &ref)
{
    for (Location_config::c_methode_vector::iterator it = ref.methode.begin(); it != ref.methode.end(); it++)
        os << "Allow_request = " << *it << std::endl;
    for (Location_config::c_str_vector::iterator it = ref.index.begin(); it != ref.index.end(); it++)
			os << "Index = " << *it << "\n";
    return (os);
}


