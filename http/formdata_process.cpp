#include "processing.hpp"

static size_t	match_boundary(const std::string& request,
						const std::string& boundary, size_t start)
{
	for (int i = 0; i < 2; i++)
		if (request[start++] != '-')
			return std::string::npos;
	return request.find(boundary, start);
}

//Checks the boundary with 2 dash before. Then checks the blankline, then the
//"Content-Disposition" header and it's value.
//Returns the position after the blankline, so the start of payload.
static size_t	is_valid_format(const std::string& request,
						const std::string& boundary, std::string& filename,
						const std::string& updir, size_t start)
{
	size_t	pos, blankline;

	filename.clear();
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
	if (pos == std::string::npos || pos > blankline)//Ou plutot pos != start+2?
		return std::string::npos;
	pos = find_nocase_header(request, "Content-Disposition:", pos);
	if (pos == std::string::npos || pos > blankline)
		return std::string::npos;
	pos = request.find("filename=\"", pos);
	if (pos != std::string::npos && pos < blankline)
	{
		pos += 10;
		start = request.find('"', pos);
		if (start != std::string::npos && start < blankline)
		{
			filename.assign(updir + "/");
			filename.append(request.substr(pos, start - pos));
		}
	}
	return blankline;
}

//Returns the end of form-data payload, which is triggered with a blankline
//plus a boundary
static size_t	end_body(const std::string& request,
						const std::string& boundary, size_t start)
{
	size_t	end = request.find("\r\n--" + boundary, start);

	if (end == std::string::npos)
	{
		end = request.find("\n--" + boundary, start);
		if (end == std::string::npos)
			return std::string::npos;
	}
	return end;
}

//Returns position of next boundary (after the blankline)
static size_t	next_boundary(const std::string& request,
						const std::string& boundary, size_t payload_end)
{
	return request.find("--" + boundary, payload_end);
}

static int		write_to_file(const std::string& request,
						const std::string& filename,
						size_t payload_start, size_t payload_end)
{
	ssize_t		ret;
	size_t		len = payload_end - payload_start;
	const char	*addr = &(request[payload_start]);
	int			fd;

	if (filename.empty())
		return -1;
	fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		return -1;
	ret = write(fd, addr, len);
	if (ret == -1)
		return -1;
	close(fd);
	return 0;
}

int				format_boundary(const std::string& value, std::string& boundary)
{
	size_t	end, start = value.find("boundary=");

	if (start == std::string::npos || start + 9 > value.size())
		return 1;
	start += 9;
	end = value.find_first_of("\r\n", start);
	if (end == std::string::npos)
		return 1;
	boundary = value.substr(start, end - start);
	return 0;
}

//processing of multipart/form-data encoding
int				formdata_process(const std::string& request,
						const std::string& value, const std::string& updir,
						size_t boundary_pos)
{
	std::string			filename, boundary;
	size_t				payload_end, payload_start;

	if (format_boundary(value, boundary))
		return 1;
	payload_start = is_valid_format(request, boundary,
							filename, updir, boundary_pos);
	while (payload_start != std::string::npos)
	{
		payload_end = end_body(request, boundary, payload_start);
		if (payload_end == std::string::npos)
			break ;
		write_to_file(request, filename, payload_start, payload_end);
		boundary_pos = next_boundary(request, boundary, payload_end);
		payload_start = is_valid_format(request, boundary,
								filename, updir, boundary_pos);
	}
	return 1;
}
