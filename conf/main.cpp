/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frfrance <frfrance@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 14:15:42 by frfrance          #+#    #+#             */
/*   Updated: 2021/08/16 21:25:58 by frfrance         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Config_base.hpp"
# include "includes/Config_struct.hpp"


int main(int ac, char **av){
	if (ac >= 2){
		std::string		file_config = av[1]; 
		Config_base	 	c_get_prsg(file_config);

		Config_struct	c_prsg = c_get_prsg.parsing_return();

		std::cout << "\n\n" << c_prsg << std:: endl;

	}
	else
		exit (0);
	
	return (0);
}

