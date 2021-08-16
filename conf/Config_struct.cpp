

# include "includes/Config_struct.hpp"

Config_struct::~Config_struct(){

}

// print operateur serveur et location 
// voir Location_config.cpp et Server_config.cpp
std::ostream& operator<<(std::ostream& os, Config_struct  &ref)
{
    for (Config_struct::c_serv_map::iterator it = ref._main_server.begin(); it != ref._main_server.end(); it++)
    {
        for(Config_struct::c_serv_list::iterator serv_it = it->second.begin(); serv_it != it->second.end(); serv_it++)                                          
            os << "\t\tSeveur  Port " << it->first << "\n" << **serv_it << "\n";
                                                             //TODO magie noir ???  
    }
    return (os);
}

