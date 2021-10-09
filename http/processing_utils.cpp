#include "webserv.hpp"

bool	get_file_content(std::string const & path, std::string & content)
{
	__D_DISPLAY("target : " << path);
	std::ifstream		file;
	std::string			line;

	if (is_dir(path))
		return false;
	file.open(path.c_str());
	if (file.fail() == true)
		return false;
	content.reserve(get_file_size(path.c_str()));
	content.append( (std::istreambuf_iterator<char>(file) ),
                (std::istreambuf_iterator<char>()    ) );
	//while (std::getline(file, line))
	//	content.append(line + "\n");
	return true;
}
