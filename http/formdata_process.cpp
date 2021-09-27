#include "processing.hpp"

size_t	match_boundary(const std::string& request,
				const std::string& boundary, size_t start)
{
	for (int i = 0; i < 2; i++)
		if (request[start++] != '-')
			return std::string::npos;
	pos = request.find(boundary, start);
	return pos;
}

//Checks the boundary with 2 dash before. Then checks the blankline, then the
//"Content-Disposition" header and it's value
size_t	is_valid_format(const std::string& request, const std::string& boundary,
				std::string& filename, size_t start)
{
	size_t	pos, blankline;

	if (start == std::string::npos)
		return std::string::npos;
	blankline = request.find("\r\n\r\n", start);
	if (blankline == std::string::npos)
	{
		if ((blankline = request.find("\n\n", start)) == std::string::npos)
			return blankline;
		blankline += 2;
	}
	else
		blankline += 4;
	if (blankline >= request.size())
		return std::string::npos;
	pos = match_boundary(request, boundary, start);
	if (pos == std::string::npos || pos > blankline)
		return std::string::npos;
	pos += boundary.size();
	if (pos + 1 < request.size())
		if (request[pos] == '-' && request[pos + 1] == '-')
			return std::string::npos;
	pos = find_nocase_header(request, "Content-Disposition:", pos);
	if (pos == std::string::npos || pos > blankline)
		return std::string::npos;
	pos = request.find("filename=\"", pos);
	if (pos != std::string::npos || pos < blankline)
	{
		pos += 9;
		start = request.find('"', pos + 1);
		if (start != std::string::npos && start < blankline)
			filename.assign(request.substr(pos, start - pos));
	}
	return blankline;
}

//detect the end of form-data body, which is a blankline plus a boundary
size_t	end_body(const std::string& request, const std::string& boundary,
				size_t start)
{
	size_t	end = request.find("\r\n\r\n--" + boundary, start);

	if (end == std::string::npos)
	{
		end = request.find("\n\n--" + boundary, start);
		if (end == std::string::npos)
			return std::string::npos;
	}
	return end;
}

size_t	write_to_file(const std::string& request, const std::string& filename,
						const std::string& boundary, size_t start, size_t end)
{
	size_t	len = end - start;
	char	*addr = &(request[start]);
	//mettre filestream

	if (!filename.empty())
	{
	}
	return request.find(boundary, end)
}

//processing of multipart/form-data encoding
int		formdata_process(Request& request, const std:string& boundary)
{
	std::string		filename;
	std::string&	request = request.getRequest();
	size_t			start, body_end, body_start = boundary.find("boundary=");

	if (body_start == std::string::npos)
		return 1;
	value.erase(0, body_start + 9);
	//ivf() retourne la position de début de body (apres blankline)
	body_start = is_valid_format(request, boundary,
							filename, request.getBodyPos());
	while (body_start != std::string::npos)
	{
		body_end = end_body(request, boundary, body_start);
		if (body_end == std::string::npos)
			break ;
		//wtf() retourne la position de début du prochain boundary
		start = write_to_file(request, filename, boundary,
						body_start, body_end);
		body_start = is_valid_format(request, boundary, filename, start);
	}
}
