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

//search into str, for exact match with header, while ignoring case, & return
//position of match.
size_t	find_nocase_header(const std::string& str,
					const std::string& header, size_t start)
{
	size_t						pos = 0 + start;
	std::string::const_iterator hdr = header.begin(), hdrend = header.end(),
								it = str.begin() + start, itend = str.end();

	if (hdr == hdrend)
		return std::string::npos;
	while (it != itend)
	{
		if (insensitive_case_match(*it, *hdr))
			if (complete_match(hdr, hdrend, it, itend))
				return pos;
		it++;
		pos++;
	}
	return std::string::npos;
}
