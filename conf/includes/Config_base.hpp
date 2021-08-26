

#ifndef	CONFIG_BASE_HPP
# define CONFIG_BASE_HPP

# include "Config_struct.hpp"
# include "Location_config.hpp"
# include "Server_config.hpp"

class	Config_base
{//---------------> CONSTRUCTEUR || DESTRUCTEUR <------------------
public :
	Config_base();
	Config_base(std::string &path);
	virtual ~Config_base();

//------------------> ENUML CONFIG NGINX <------------------
	enum conf_nginx{
		n_root = 0,
		n_index,
		n_server,
		n_listen,
		n_body,
		n_cgi_ext,
		n_location,
		n_auto_index,
		n_error_page, 
		n_name,
		n_allow_request,
		n_upload_d,
		n_auth_b,
		n_braket,
		n_none,
		n_bracket_error,
	};

//------------------------> TYPE DEF  <-----------------------
		typedef Config_struct:: c_name_vector			c_name_vector;
		typedef Config_struct::c_methode_vector			c_methode_vector;
		typedef Config_struct::c_cgi_map				c_cgi_map;
		typedef Config_struct::c_error_map				c_error_map;
		typedef Config_struct::p_listen					p_listen;
		typedef Config_struct::c_serv_vector			c_serv_vector;

//-------------------> FUNC PUBLIC  <-----------------------	
		c_serv_vector		*get_vector() const ;
		Config_struct		parsing_return() const ;
	
//------------------> PRIVATE FONCTION <------------------
private :
		void				open_conf();
		void				prsg_main();
		conf_nginx			enum_prsg(std::string &str);
		
		void				verif_serveur();	
		void				verif_location();
		void 				locat_bracket(std::string &buff);
		conf_nginx			verif_serv_listen(std::string &str, std::string &conf);
		conf_nginx			verif_locat(std::string &str, std::string &conf);
		conf_nginx			verif_locat_bracket(std:: string &str, std::string &conf);
		void				in_server();
		void				in_location();	
		void				verif_container();

		void				get_container(conf_nginx &conf, std::string &str);
		void 				listen_prsg(std::string &str, p_listen &prsg);
		void				name_serv_prsg(std::string &str, c_name_vector &prsg);
		void				basic_prsg(std::string &str, std::string &prsg);
		void 				cgi_ext_prsg(std::string &str, c_cgi_map &prsg);
		void				methode_prsg(std::string &str, c_methode_vector &prsg);
		void				auto_index_prsg(std::string &str, bool &prsg);
		void				error_page_prsg(std::string &str, c_error_map &prsg);
		void				index_prsg(std::string &str, std::string &prsg);
		void				body_size_prsg(std::string &str, size_t &prsg);

		void				init_value(std:: string &path);
		void				ft_trim(std::string &str);
		void 				print_error(const std::string str) const ;
		conf_nginx			case_conf(const std::string &conf) const;
		void				find_and_replace(std::string &str, std::string &src, std::string &dest);
		int					search_space(std::string &str);
		int 				error_methode(std::string &temp);
		int					error_semilicon(std::string &str);
		int					conver_to_str(std::string &str);

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

		bool				braket_on;
		size_t				space_count;	
};

# endif
