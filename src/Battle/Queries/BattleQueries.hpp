#pragma once

#include <Battle/Queries/CellOccupancy.hpp>
#include <Battle/Queries/CombatQueries.hpp>
#include <Battle/Queries/MarchMovement.hpp>

namespace sw::battle
{
	class UnitRegistry;
	class Map;

	/** @brief Фасад доменных запросов над UnitRegistry и Map. */
	class BattleQueries
	{
	public:
		BattleQueries(const UnitRegistry& units, const Map& map);

		CellOccupancy& getOccupancy() { return _occupancy; }
		const CellOccupancy& getOccupancy() const { return _occupancy; }

		CombatQueries& getCombat() { return _combat; }
		const CombatQueries& getCombat() const { return _combat; }

		MarchMovement& getMarch() { return _march; }
		const MarchMovement& getMarch() const { return _march; }

	private:
		CellOccupancy _occupancy;
		CombatQueries _combat;
		MarchMovement _march;
	};
}
