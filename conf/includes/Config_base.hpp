
#ifndef	CONFIG_BASE_HPP
# define CONFIG_BASE_HPP

# include "Config_struct.hpp"
# include "Location_config.hpp"
# include "Server_config.hpp"

class	Config_base
{//---------------> CONSTRUCTEUR || DESTRUCTEUR <------------------
public :
	Config_base(std::string &path);
	virtual ~Config_base();

//------------------> ENUML CONFIG NGINX <------------------
	enum conf_nginx{
		n_root = 0,
		n_index,
		n_server,
		n_listen,
		n_cgi_ext,
		n_location,
		n_auto_index,
		n_error, 
		n_name,
		n_allow_request,
		n_braket,
		n_none
	};

//------------------------> TYPE DEF  <-----------------------
		typedef Config_struct::c_str_vector				c_str_vector;
		typedef Config_struct::c_cgi_map				c_cgi_map;
		typedef Config_struct::c_methode_vector			c_methode_vector;
		typedef Config_struct::c_host_vector			c_host_vector;									
		
		typedef Config_struct::c_serv_list				c_serv_list;
		typedef	Config_struct::serv_port				serv_port;
		typedef std::map<serv_port, c_serv_list>		c_srv_map;

//-------------------> FUNC PUBLIC  <-----------------------	
		Config_struct		parsing_return();
	
//------------------> PRIVATE FONCTION <------------------
private :
		void				open_conf();
		void				prsg_main();
		conf_nginx			enum_prsg(std::string &str);
	
		void				verif_serveur();	
		void				verif_location();
		void 				locat_bracket(std::string &buff);
		void				in_server();
		void				in_location();	

		void				get_container(conf_nginx &conf, std::string &str);
		void 				host_prsg(std::string &buff, c_host_vector &host, serv_port &port);
		void				name_serv_prsg(std::string & buff, c_str_vector &dest);
		void				root_prsg(std::string &buff, std::string &dest);
		void 				cgi_ext_prsg(std::string &buff, c_cgi_map &dest);
		void				methode_prsg(std::string &buff, c_methode_vector &dest);
		void				auto_index_prsg(std::string &str, bool &prsg);
		void 				location_prsg(const std::string &buff, bool & dest);
		void				index_prsg(std::string & buff, c_str_vector &dest);

		void				init_value(std:: string &path);
		void				ft_trim(std::string &str);
		void 				print_error(std::string str);
		void				find_and_replace(std::string &str, std::string &src, std::string &dest);

//------------------> PRIVATE VARIABLE <------------------
		Location_config		*_location;  //-> tous les locations nginx
		Server_config		*_server; //-> tous mon serveur nginx
		Config_struct		_main_config;
		
		bool				_bool_serv;
		bool				_bool_locat;
		bool				_again;
		size_t				_error;

		std::ifstream		_file;
		std::string			_config;
};

# endif
