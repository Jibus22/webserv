

# include "includes/Config_struct.hpp"

Config_struct::Config_struct() : _main_server(new c_serv_vector){

}



Config_struct::~Config_struct(){
//TODO  delet iterator vector 
    
    
    
}

// print operateur serveur et location 
// voir Location_config.cpp et Server_config.cpp
std::ostream& operator<<(std::ostream& os, Config_struct  &ref){
    for (Config_struct::c_serv_vector::iterator it = ref._main_server->begin(); it != ref._main_server->end(); it++){
        os  << **it;
        std::cout << "--------------------------------------------" << "\n";
    }
    return (os);
}

