#include "webserv.hpp"

//MAJ: 65-90
//MIN: 97-122  (+32)
bool	insensitive_case_match(char a, char b)
{
	if (a == b)
		return true;
	else if (isalpha(a) && isalpha(b))
	{
		if (islower(a))
		{
			if (isupper(b))
				if ((b + 32) == a)
					return true;
		}
		else
		{
			if (islower(b))
				if ((b - 32) == a)
					return true;
		}
	}
	return false;
}

bool	complete_match(std::string::const_iterator hdr,
						std::string::const_iterator& hdrend,
						std::string::const_iterator i,
						std::string::const_iterator& iend)
{
	while (i != iend)
	{
		if (hdr == hdrend)
			return true;
		if (insensitive_case_match(*i, *hdr) == false)
			return false;
		hdr++;
		i++;
	}
	return false;
}

size_t	find_nocase_header(const std::string& str, const std::string& header)
{
	size_t						pos = 0;
	std::string::const_iterator hdr = header.begin(), hdrend = header.end(),
								i = str.begin(), iend = str.end();

	if (hdr == hdrend)
		return std::string::npos;
	while (i != iend)
	{
		if (insensitive_case_match(*i, *hdr))
			if (complete_match(hdr, hdrend, i, iend))
				return pos;
		i++;
		pos++;
	}
	return std::string::npos;
}
