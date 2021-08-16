# include "includes/Config_base.hpp"
# include "includes/Config_struct.hpp"
# include "includes/Location_config.hpp"


Config_base::Config_base(std::string &config){
	init_value(config);
	open_conf();  // -> ouverture fichier
	prsg_main();	
	_file.close();
}

Config_base::~Config_base(){}

void				Config_base::prsg_main(){
	std::string		str;

	while (getline(_file, str)){ // envois les caractères du fichier dans str
		_error++;

		ft_trim(str); 
		if (str.empty())
			continue ;
	
		conf_nginx	conf = enum_prsg(str);	// -> conf va etre egal a server, listen ect..

		if (conf == n_none)
			print_error("no parametre");

		
		if (conf == n_server) {		// -> en cours 
 			in_server();
			continue;
		}
		else if (conf == n_location){
			in_location();
			locat_bracket(str);
			continue ;
		}
	
		if (conf == n_braket) {		// -> Si bracket de fin 
	
			if (_bool_locat == true){
				verif_location();
				_server->location[_location->uri] = _location;
				_location = NULL;
				_bool_locat = false;	
				_again = true;
			}
			else if (_bool_serv == true){	// -> en cours 
				if (_server->location.empty())
					verif_serveur();
				_main_config._main_server[_server->port].push_back(_server);
				_server = NULL;
				_bool_serv = false;
				_again = false;
			}
			else {
				print_error("bracket");	 // -> en cours

			}
				
		}
		if (_bool_locat == true)
			if (conf == n_name || conf == n_listen)
				print_error("invalid param dans location");

		get_container(conf, str);			
	}
}

//---------------------------------------------------------------------------------------//
//------------------------------ PARSING ARG FICHIER CONF -------------------------------//
//---------------------------------------------------------------------------------------//
void			Config_base::get_container(conf_nginx &conf, std::string &str){

		
// arguments du fichier (127.0.2, GET..) dans des variable/container		
// voir fichier Config_struct pour les types 

	switch (conf){
		case n_listen :
			host_prsg(str, _server->host, _server->port);
			break;
		case n_name :
			name_serv_prsg(str, _server->name);
			break;
		case n_root :
			if (_server->root.empty() == false)
					print_error("Rediff parametres");
			root_prsg(str, _server->root);
			break ;
		case n_index :
			if (_bool_serv)
				index_prsg(str, _server->index);
			else
				index_prsg(str, _location->index);
			break;
		case n_cgi_ext :
			cgi_ext_prsg(str, _server->cgi);
			break;
		case n_allow_request :
			methode_prsg(str, _location->methode);
			break ;	
		case n_auto_index :
			if (_bool_locat)
				auto_index_prsg(str, _location->auto_index);
			else
				auto_index_prsg(str, _server->auto_index);
		default :
				break;
		}
}


void 		Config_base::host_prsg(std::string &str, c_host_vector &host, serv_port &port) {
	size_t		position = str.find(':');
	std::string host_str;

	host_str = str.substr(0, position);

// si pas de directive listen
	if (position == std::string::npos || position == 0)
		host_str = "0.0.0.0";
		
	else 
		host.push_back(host_str);

	if (position != std::string::npos)
		str.erase(0, position + 1);

// convert string en int 
	std::stringstream ss;  
  	ss << str;  
  	ss >> port; 
}

void		Config_base::name_serv_prsg(std::string & str, c_str_vector &prsg){
	size_t		position;

	while ((position = str.find_first_of(" \t")) != std::string::npos){
		std::string temp = str.substr(0, position);
		prsg.push_back(temp);
		position = str.find_first_not_of(" \t", position);
		str.erase(0, position);
	}
	if (str.size()){
		prsg.push_back(str);
	}

}

void		Config_base::methode_prsg(std::string &str, c_methode_vector &prsg){
	size_t		position;

	while ((position = str.find_first_of(" \t")) != std::string::npos){
		std::string temp = str.substr(0, position);
		prsg.push_back(temp);
		position = str.find_first_not_of(" \t", position);
		str.erase(0, position);
	}
	if (str.size())
		prsg.push_back(str);
}

void		Config_base::index_prsg(std::string & str, c_str_vector &prsg){
	size_t		position;

	while ((position = str.find_first_of(" \t")) != std::string::npos){
		std::string temp = str.substr(0, position);
		prsg.push_back(temp);
		position = str.find_first_not_of(" \t", position);
		str.erase(0, position);
	}
	if (str.size()){
		prsg.push_back(str);
	}
}

void 		Config_base::root_prsg(std::string &str, std::string &prsg){
	prsg = str;
}

void 		Config_base::cgi_ext_prsg(std::string &str, c_cgi_map &prsg)
{
	size_t		position = str.find_first_of(" \t");

	if (position == std::string::npos)
		print_error("Executable mannquant");

	std::string p = str.substr(0, position); // -> point de lexec 

	position = str.find_first_not_of(" \t", position);
	str.erase(0, position);

	prsg[p] = str;



}

void	Config_base::auto_index_prsg(std::string &str, bool &prsg){ 
	
// -> en cours 
	(void)str;
	(void)prsg;

}


Config_base::conf_nginx		Config_base::enum_prsg(std:: string &str){
	size_t		pos = str.find_first_of(" \t");

	std::string conf = str.substr(0, pos);

	if (conf[0] == '}')
		return (n_braket);
	
	pos = str.find_first_not_of(" \t", pos);
	str.erase(0, pos);
	size_t conf_size = conf.size();
	
	// std::cout << conf << " " << str << std::endl;  //-> Print fichier nginx 

// number case = size conf (index = 5, location = 8)
	switch (conf_size){		
		case 4 :
			return	(conf == "root" ? n_root : n_none);
		case 5 :
			return	(conf == "index" ? n_index : n_none);
		case 6 :
			return	(conf == "server" ? n_server : conf == "listen" ? n_listen : n_none);
		case 7 :
			return	(conf == "cgi_ext" ? n_cgi_ext : n_none);
		case 8 :
			return	(conf == "location" ? n_location : n_none);
		case 9 :
			return	(conf == "autoindex" ? n_auto_index : n_none);
		case 10 :
			return	(conf == "error_page" ? n_error : n_none);
		case 11 :
			return	(conf == "server_name" ? n_name : n_none);
		case 13 :
			return	(conf == "allow_request" ?  n_allow_request : n_none);
		default :
			return (n_none);
			break;
	}
}

//---------------------------------------------------------------------------------------//
//--------------------------------- VERIF SERV LOCTION ----------------------------------//
//---------------------------------------------------------------------------------------//
void			Config_base::verif_serveur(){  // -> en cours 
	if (_server->host.empty())
		print_error("Listen manquant ");
	if (_server->root.empty())
		print_error("Root manquant");

// Si server_name vide -> Va prendre listen (127.0.0.1)
	if (_server->name.empty()){
		_server->name = _server->host;
	}
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

	if (_location->root.empty()) 
	{
		_location->root = _server->root + one + _location->uri;
		find_and_replace(_location->root, two, one);
	}

	if (_location->methode.empty()){
		print_error("methode manquant");
		return ;
	}
	
}

void Config_base::locat_bracket(std::string &str){ 
	size_t		pos = str.find_first_of(" {");

// Vérifier s'il manque un URI
	if (pos == 0)
		print_error("URI manquant");
	_location->uri = str.substr(0, pos);
}

void			Config_base::in_server(){
	if (_bool_serv == true) 
				print_error("Serveur deja open"); 
			_server = new Server_config();
			_bool_serv = true;
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

Config_struct Config_base::parsing_return(){ // return dans le main mon objet du parsing
	return (_main_config);
}

//---------------------------------------------------------------------------------------//
//----------------------------------- OTHER FUNCTION ------------------------------------//
//---------------------------------------------------------------------------------------//
void				Config_base::ft_trim(std::string& str){
	if (str.empty())
		return ;
	size_t			i = str.find_first_not_of(" \t");
	size_t			j = str.find_last_of(";{}");

	if (j == std::string::npos)
		print_error("missing ; or {");
	str = str.substr(i, j + 1);
}

void
Config_base::find_and_replace(std::string &str, std::string &src, std::string &dest)
{
	std::string::size_type	find;

	while ((find = str.find(src)) != std::string::npos)
	{
		str.replace(find, src.length(), dest);
	}
}


void Config_base::print_error(std::string str) 
{
	std::cout << "\033[1;33m" ;
	std::cout << "Error line " << _error << " [ "<< str << " ]" << "\n";
	std::cout << "\033[0m";
	exit (0);
}



