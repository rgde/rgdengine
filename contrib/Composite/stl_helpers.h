#pragma once

namespace stdh
{

	template<class T>
	void erase(std::vector<T> container, T& value)
	{
		std::vector<T>::iterator _where = std::find(container.begin(), container.end(), value);
		if (_where != container.end())
			container.erase(_where);
	}

}
