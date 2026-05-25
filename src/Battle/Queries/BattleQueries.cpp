#include "BattleQueries.hpp"

#include <Battle/Units/UnitRegistry.hpp>
#include <Battle/Map.hpp>

namespace sw::battle
{
	BattleQueries::BattleQueries(const UnitRegistry& units, const Map& map)
		: _occupancy(units, map)
		, _combat(units)
		, _march(units, map, _occupancy)
	{
	}
}
