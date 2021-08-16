/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location_config.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frfrance <frfrance@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 16:19:13 by frfrance          #+#    #+#             */
/*   Updated: 2021/08/16 21:25:52 by frfrance         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


