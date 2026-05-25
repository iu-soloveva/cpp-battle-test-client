#include "Map.hpp"

namespace sw::battle
{
	void Map::reset()
	{
		_width = 0;
		_height = 0;
	}

	void Map::create(uint32_t width, uint32_t height)
	{
		_width = width;
		_height = height;
	}

	bool Map::isInside(uint32_t x, uint32_t y) const
	{
		return x < _width && y < _height;
	}
}
