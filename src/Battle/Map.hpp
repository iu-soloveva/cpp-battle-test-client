#pragma once

#include <cstdint>

namespace sw::battle
{
	/** @brief Размеры карты и базовые граничные проверки. */
	class Map
	{
	public:
		void reset();

		void create(uint32_t width, uint32_t height);

		bool isInside(uint32_t x, uint32_t y) const;

	public:
		bool hasMap() const { return _width > 0 && _height > 0; }

		uint32_t getWidth() const { return _width; }
		uint32_t getHeight() const { return _height; }

	private:
		uint32_t _width{};
		uint32_t _height{};
	};
}
