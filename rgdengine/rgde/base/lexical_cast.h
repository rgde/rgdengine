#pragma once

namespace base
{
	template<typename T> 
	inline void lower_case(T &src)
	{
		std::transform(src.begin(), src.end(), src.begin(), tolower);
	}

	template<typename T> 
	inline T lower_case(const T &src)
	{
		T result = src;
		std::transform(result.begin(), result.end(), result.begin(), tolower);
		return result;
	}

	template<typename T> 
	inline void upper_case(T &src)
	{
		std::transform(src.begin(), src.end(), src.begin(), toupper);
	}

	template<typename T> 
	inline T upper_case(const T &src)
	{
		T result = src;
		std::transform(result.begin(), result.end(), result.begin(), toupper);
		return result;
	}

	template<typename Target, typename Source> 
	inline Target lexical_cast(const Source &src)
	{
		std::stringstream stream;
		stream << src;
		Target dst;
		stream >> dst;
		return dst;
	}

	template<> 
	inline bool lexical_cast<bool, std::string>(const std::string &src)
	{
		if(lower_case<std::string>(src) == "true")
			return true;
		else
			return false;
	}

	template<> 
	inline std::string lexical_cast<std::string, std::string>(const std::string &src)
	{
		return src;
	}

	template<typename T> 
	inline void find_replace(T &src, const T &what, const T &with)
	{
		typename T::size_type offset = 0;
		while ((offset = src.find(what, offset)) != T::npos)
		{
			src.replace(offset, what.length(), with);
			offset += what.length();
		}
	}

	template<class Ch>
	std::list<std::basic_string<Ch> > tokenize(const std::basic_string<Ch>& delim, const std::basic_string<Ch> &src)
	{
		std::list<std::basic_string<Ch> > out_tokens;

		for (size_t end	= 0; end != std::basic_string<Ch>::npos;)
		{
			size_t start = src.find_first_not_of(delim, end);
			end = src.find_first_of(delim, start);

			if (start != std::string::npos)
			{
				size_t end_pos = end == std::string::npos ? end : end - start;
				out_tokens.push_back(src.substr(start, end_pos));
			}
		}
		return out_tokens;
	}
}