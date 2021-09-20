

#include "includes/Config_base.hpp"
# include "includes/Config_struct.hpp"


int main(int ac, char **av){
	if (ac == 2){
		std::string		file_config = av[1]; 
		Config_base	 	c_get_prsg(file_config);

		Config_struct	c_prsg = c_get_prsg.parsing_return();


		std::cout << "\n\n" << c_prsg << std:: endl;

	}
	else 
	{
		std::cout << "Arguments invalide\n";
		return (1); 
	}
	return (0);
}

