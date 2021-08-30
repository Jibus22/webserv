# include "includes/Config_base.hpp"
# include "includes/Config_struct.hpp"
# include "includes/Location_config.hpp"


Config_base::Config_base(std::string &config){
	init_value(config);
	open_conf();
	prsg_main();	
	_file.close();
}

Config_base::~Config_base(){}

void				Config_base::prsg_main(){
	std::string		str;

	while (getline(_file, str)){
		_error++;
		braket_on = false;
		ft_trim(str); 
		if (str.empty())
			continue ;
		conf_nginx	conf = enum_prsg(str);
		if (conf == n_bracket_error)
			continue;
		if (conf == n_none)
			print_error("no parametre");
		if (conf == n_server) {		
 			in_server();
			continue;
		}
		else if (conf == n_location){
			in_location();
			locat_bracket(str);
			continue ;
		}
		if (conf == n_braket) {	
			if (_bool_locat == true){
				verif_location();
				_server->location[_location->uri] = _location;
				_location = NULL;
				_bool_locat = false;	
				_again = true;
			}
			else if (_bool_serv == true){
				if (_server->location.empty())
					verif_serveur();
				_main_config._main_server->push_back(_server);
				_server = NULL;
				_bool_serv = false;
				_again = false;
			}
			else {
				print_error("bracket");	
			}	
		}
		if (_bool_locat == true)
			if (conf == n_name || conf == n_listen || conf == n_body || conf == n_error_page)
				print_error("invalid param must be server");

		get_container(conf, str);
	}
}

//---------------------------------------------------------------------------------------//
//------------------------------ PARSING ARG FICHIER CONF -------------------------------//
//---------------------------------------------------------------------------------------//
void			Config_base::get_container(conf_nginx &conf, std::string &str){
	switch (conf){
		case n_listen :
			if (_bool_serv)
				listen_prsg(str, _server->listen);
			else
				print_error("listen  must be server");
			break;
		case n_name :
			if (_bool_serv)
				name_serv_prsg(str, _server->name_serv);
			else
				print_error("server name  must be server");
			break;
		case n_error_page :
			if (_bool_serv)
				error_page_prsg(str, _server->error_page);	
			else
				print_error("error_page must be server");
			break;
		case n_root :
			if (_bool_locat)
				basic_prsg(str, _location->root);
			else
				print_error("Root must be location");
			break ;
		case n_index :
			if (_bool_locat)
				index_prsg(str, _location->index);
			else
				print_error("Index must be location");
			break;
		case n_cgi_ext :
			if (_bool_locat)
				cgi_ext_prsg(str, _location->cgi);
			else
				print_error("CGI must be location");
			break;
		case n_allow_request :
			if (_bool_locat)
				methode_prsg(str, _location->methode);
			else
				print_error("Allow_request must be location");
			break ;	
		case n_auto_index :
			if (_bool_locat)
				auto_index_prsg(str, _location->auto_index);
			else
				print_error("Auto index must be location");
			break;
		case n_body :
			if (_bool_serv)
				body_size_prsg(str, _server->m_body_size);
			else
				print_error("client_max_body_size must be server");
			break;
		case n_upload_d :
			if (_bool_locat)
				basic_prsg(str, _location->upload_dir);
			else
				print_error("upload_dir must be location");	
		default :
				break;
		}
}

void			Config_base::listen_prsg(std::string &str, p_listen &prsg){
	size_t			position = str.find(':');
	size_t			db = std::count(str.begin(), str.end(), ':');
	std::string		port;

	if (search_space(str) == 1 || error_semilicon(str) == 1)
		print_error("No space in listen");
	if (db > 1)
		print_error("to many separate ':'");
	if (str[str.size() - 1] == ';')
		str = ft_pop_back(str);
	std::string 	host_str = str.substr(0, position);
	if (host_str.empty())
		prsg.first = "0.0.0.0";
	else
		prsg.first = host_str;
	if (db < 1)
		prsg.first = str;
	if (position != std::string::npos){
		str.erase(0, position + 1);
		port = str;
		for (int i = 0; i < port[i]; i++)
			if (!std::isdigit(str[i]))
				print_error("only number is port");
	}



	if (port.empty())
		prsg.second = 80;
	else
		prsg.second = conver_to_str(port);
}


std::string Config_base::ft_pop_back(std::string str){
	size_t			position = str.size() - 1;

	std::string temp; temp = str.substr(0, position);
	return (temp);
}


void		Config_base::name_serv_prsg(std::string &str, c_name_vector &prsg){
	size_t		position;
	size_t		semilicon = 0;

	if (error_semilicon(str) == 1)
		print_error("No space after semicolon");
	if (!str.empty()) 
        str = ft_pop_back(str);
	while ((position = str.find_first_of(" \t")) != std::string::npos){
		std::string temp = str.substr(0, position);
		prsg.push_back(temp);
		position = str.find_first_not_of(" \t", position);
		str.erase(0, position);
	}
	if (str.size()){	
		for (int i = 0; i < str[i]; i++)
			if (str[i] == ';')
				semilicon = 1;
		if (semilicon == 0)
			prsg.push_back(str);
	}
}

void		Config_base::methode_prsg(std::string &str, c_methode_vector &prsg){
	size_t		position;
	size_t		semilicon = 0;

	if (error_semilicon(str) == 1)
		print_error("No space after semicolon");
	if (!str.empty()) 
        str = ft_pop_back(str);
	while ((position = str.find_first_of(" \t")) != std::string::npos){
		std::string temp = str.substr(0, position);
		if (temp[temp.size() - 1] == ';')
			temp = ft_pop_back(temp);
		if (error_methode(temp) == 1)
			print_error("Error methode");
		prsg.push_back(temp);
		position = str.find_first_not_of(" \t", position);
		str.erase(0, position);
	}
	if (str.size()){	
		if (error_methode(str) == 1)
			print_error("Error methode");
		for (int i = 0; i < str[i]; i++)
			if (str[i] == ';')
				semilicon = 1;
		if (semilicon == 0)
			prsg.push_back(str);
	}
}

void		Config_base::index_prsg(std::string &str, std::string &prsg){
	size_t		position;

	if (error_semilicon(str) == 1)
		print_error("No space after semicolon");
	if (search_space(str) == 1)
		print_error("No space arguments");
	while ((position = str.find_first_of(" \t")) != std::string::npos){
		std::string temp = str.substr(0, position);
		prsg = temp;
		position = str.find_first_not_of(" \t", position);
		str.erase(0, position);
	}
	if (str.size()){
		if (str[str.size() - 1] == ';')
			str = ft_pop_back(str);
		prsg = str;
	}
}

void 		Config_base::basic_prsg(std::string &str, std::string &prsg) {
	if (error_semilicon(str) == 1)
		print_error("No space after semicolon");
	if (search_space(str) == 1)
		print_error("No space arguments");
	if (prsg.empty() == false)
		print_error("Parametre deja existant");
	if (str[str.size() - 1] == ';'){
		str = ft_pop_back(str);
	}
	prsg = str;	
	
}

void 		Config_base::cgi_ext_prsg(std::string &str, c_cgi_map &prsg){
	size_t		position = str.find_first_of(" \t");
	
	if (error_semilicon(str) == 1)
		print_error("No space after semicolon");
	if (position == std::string::npos)
		print_error("Executable mannquant");
	std::string p = str.substr(0, position); 
	position = str.find_first_not_of(" \t", position);
	str.erase(0, position);
	if (search_space(str) == 1)
		print_error("No space arguments");
	if (str[str.size() - 1] == ';')
		str = ft_pop_back(str);
	prsg[p] = str;
}

void		Config_base::error_page_prsg(std::string &str, c_error_map &prsg){
	size_t		position = str.find_first_of(" \t");
	int			nb = 0;

	if (error_semilicon(str) == 1)
		print_error("No space after semicolon");
	if (position == std::string::npos)
		print_error("error -> error_page");
	std::string number_error = str.substr(0, position); 
	for (size_t i = 0; i < number_error.length(); i++)
		if (std::isalpha(number_error[i]))
			print_error("Only number");
	position = str.find_first_not_of(" \t", position);
	str.erase(0, position);
	if (search_space(str) == 1)
		print_error("No space arguments");
	if (str[0] == ';')
		print_error("Error No path");
	if (str[str.size() - 1 ] == ';')
		str = ft_pop_back(str);
	std::stringstream ss;  
  	ss << number_error;  
  	ss >> nb; 
	prsg[nb] = str;
}

void			Config_base::body_size_prsg(std::string &str, size_t &prsg){
	std::string 	letter;
	std::string 	number;
	size_t			memory = 1;
	size_t			arg = 0;
	size_t			nb;

	if (error_semilicon(str) == 1)
		print_error("No space after semicolon");
	if (search_space(str) == 1)
		print_error("No space arguments");
	for (size_t i = 0; i < str.length(); i++){
		if (std::isalpha(str[i])){
			letter = str.substr(i, str.size());
			number = str.substr(0, i);
			break;
		}
	}
	if (letter[0] == 'K' || letter[0] ==  'k')
		memory = 100; 
	else if (letter[0] == 'M' || letter[0] == 'm')
		memory = 1000000;
	else
		arg = 1;
	if (letter.size() > 3 || std::isalpha(str[0]))
		print_error("Error client_max_body_size");
	if (arg == 1){
		if (std::isalpha(letter[0]))
			print_error("client_max_body_size");
		std::stringstream ss;  
  		ss << str;  
  		ss >> nb; 
	}
	else{
		std::stringstream ss;  
  		ss << number;  
  		ss >> nb; 
	}
	prsg = nb * memory;
}

void		Config_base::auto_index_prsg(std::string &str, bool &prsg){ 
	if (error_semilicon(str) == 1)
		print_error("No space after semicolon");
	if (str[str.size() - 1] == ';')
		str = ft_pop_back(str);
	if (str == "off")
		prsg = false;
	else if (str == "on")
		prsg = true;
	else
		print_error("Error auto index");
}


Config_base::conf_nginx		Config_base::enum_prsg(std:: string &str){
	size_t				pos = str.find_first_of(" \t");
	size_t				b = str.find_first_of("{");
	std::string 		conf;

	if (str == "server{"){
		conf = str.substr(0, b);
		pos = str.find_last_of('{');
	}
	else {
		conf = str.substr(0, pos);
		pos = str.find_first_not_of(" \t", pos);
	}
	if (conf[0] == '}')
		return (n_braket);
	str.erase(0, pos);
	size_t conf_size = conf.size();
	// std::cout << "conf = " << conf << " \t\tstr = " << str << std::endl;  //-> Print fichier nginx 
	if (conf.empty() && !str.empty())
		print_error("Error bracket or semicolon");
	switch (conf_size){		
		case 4 :
			return	(conf == "root" ? n_root : n_none);
		case 5 :
			return	(conf == "index" ? n_index : n_none);
		case 6 :
			return (verif_serv_listen(str, conf));
		case 7 :
			return	(conf == "cgi_ext" ? n_cgi_ext : n_none);
		case 8 :
			return (verif_locat(str, conf));
		case 9 :
			return	(conf == "autoindex" ? n_auto_index : n_none);
		case 10 :
			return	(case_conf(conf));
		case 11 :
			return	(conf == "server_name" ? n_name : n_none);
		case 13 :
			return	(conf == "allow_request" ?  n_allow_request : n_none);
		case 20 :
			return (conf == "client_max_body_size" ? n_body : n_none);
		default :
			if (space_count == 1)
				return (n_bracket_error);
			return (n_none);
			break;
	}
}

//---------------------------------------------------------------------------------------//
//--------------------------------- VERIF PARSING ---------------------------------------//
//---------------------------------------------------------------------------------------//
void			Config_base::verif_serveur(){  // -> en cours 
	if (_server->listen.first.empty()){
		_server->listen.first = "0.0.0.0";
		_server->listen.second = 80;
	}
	if (_server->name_serv.empty())
		_server->name_serv.push_back("");
	if (!_server->m_body_size)
		_server->m_body_size = 1000000;
}

void		Config_base::in_location(){	
	if (_bool_serv == false)
		print_error("définition de LOCATION hors du serveur");
	if (_server->location.empty())
			verif_serveur();
	if (_bool_locat == true)
			print_error("définition de LOCATION dans location"); 
	_location = new Location_config();
	_bool_locat = true;
	_again = false;
}

void				Config_base::verif_location(){ // -> en cours 
// Définir la racine avec la racine du serveur + uri 
// si aucune redéfinition de la racine à l'emplacement
	std::string one = "/";
	std::string two = "//";
	if (_location->root.empty()) {
		_location->root = one + _location->uri;
		find_and_replace(_location->root, two, one);
	}
	if (_location->methode.empty())
		_location->methode.push_back("GET");
	if (_location->index.empty())
		_location->index = "index.html";
}

Config_base::conf_nginx		Config_base::verif_locat(std::string &str, std::string &conf){
	size_t 			bracket = std::count(str.begin(), str.end(), '{');

	if (conf == "location") {
		if (bracket < 1 )
			print_error("mauvais bracket location");
		return (n_location);
	}
	return	(n_none);
}

void 				Config_base::locat_bracket(std::string &str){ 
	size_t			pos = str.find_first_of(" {");
	size_t			b = str.find_last_of("{}");
	std::string 	bracket;
	size_t 			n = std::count(str.begin(), str.end(), '{');

	bracket = str.substr(b, std::string::npos);
	if (bracket == "}" || n > 1)
		print_error("bracket error");
	if (pos == 0)
		print_error("URI manquant");
	_location->uri = str.substr(0, b);
	if (search_space(_location->uri) == 1)
		print_error("No space argument");
}

void			Config_base::in_server(){
	if (_bool_serv == true) 
		print_error("Serveur deja open"); 
	_server = new Server_config();
	_bool_serv = true;
}

Config_base::conf_nginx		Config_base::verif_serv_listen(std::string &str, std::string &conf) {
	if (conf == "listen")
		return (n_listen);
	if (conf == "server" ) {
		if (str != "{")
			print_error("mauvais bracket server");
		return (n_server);
	}
	return	(n_none);
}
//---------------------------------------------------------------------------------------//
//----------------------------------- FILL INIT OPEN ------------------------------------//
//---------------------------------------------------------------------------------------//

void				Config_base::open_conf(){
	_file.open(_config.c_str()); // Renvoie un pointeur vers un tableau

	if (_file.fail() == true)
		throw std::runtime_error("Open file");
}

void				Config_base::init_value(std::string &config){
	_config = config;
	_bool_serv = false;
	_bool_locat = false;
	_location = NULL;
	_server = NULL;
	_again = false;
	_error = 0;
}

Config_struct Config_base::parsing_return() const { 
	return (_main_config);
}

Config_struct::c_serv_vector *Config_base::get_vector() const {
	return (_main_config._main_server);
}

//---------------------------------------------------------------------------------------//
//----------------------------------- OTHER FUNCTION ------------------------------------//
//---------------------------------------------------------------------------------------//
void				Config_base::ft_trim(std::string& str){
	if (str.empty())
		return ;
	size_t			i = str.find_first_not_of(" \t");
	size_t			j = str.find_last_of(";{}");
	size_t			db = std::count(str.begin(), str.end(), '}');
	
	space_count = 0;
	if (db > 1)
		print_error("bracket error");
	if (j == std::string::npos) 
		space_count = 1; 
	else
		str = str.substr(i, j + 1);
}

void 		Config_base::find_and_replace(std::string &str, std::string &src, std::string &dest)
{
	std::string::size_type	find;
	while ((find = str.find(src)) != std::string::npos){
		str.replace(find, src.length(), dest);
	}
}

Config_base::conf_nginx		Config_base::case_conf(const std::string &conf) const {
	if (conf == "error_page")
		return (n_error_page);
	else if (conf == "upload_dir")
		return (n_upload_d);
	else
		return (n_none);
}

int			Config_base::search_space(std::string &str){
	size_t 		space = std::count(str.begin(), str.end(), ' ');
	if (space >= 1)
		return (1);
	return (0);
}


int			Config_base::error_semilicon(std::string &str){
	size_t		error;
	error = str.find_first_of(";");

	std::string temp = str.substr(error, std::string::npos);
	if (search_space(temp) == 1)
		return (1);
	return (0);

}

int 		Config_base::error_methode(std::string &temp){
	if ((temp == "GET" || temp == "POST" || temp == "DELETE"))
		return (0);
	else if ((temp != "GET" || temp != "POST" || temp != "DELETE"))
		return (1);
	return (0);
}

int		Config_base::conver_to_str(std::string &str){
	int 	nb;
	std::stringstream ss;  
  		ss << str;  
  		ss >> nb;	
	return (nb);
}



void 		Config_base::print_error(const std::string str) const {
	std::cout << "\033[1;33m" ;
	std::cout << "Error line " << _error << " [ "<< str << " ]" << "\n";
	std::cout << "\033[0m";
	exit (0);
}

