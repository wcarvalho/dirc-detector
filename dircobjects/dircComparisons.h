#include <vector>

namespace dirc{

	template < typename T>
	bool withinrange(std::vector<T>& range, T value){
		if ( (value < range.at(0)) || (value > range.at(1)) )
			return false;
		else return true;
	}

}