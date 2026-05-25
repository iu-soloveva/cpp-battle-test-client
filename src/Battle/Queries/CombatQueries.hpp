#pragma once

#include <Battle/Units/Components/Types.hpp>

#include <cstdint>
#include <vector>

namespace sw::battle
{
	class UnitRegistry;

	using EntityId = components::EntityId;

	/** @brief Боевые spatial-запросы: дистанция и поиск целей. */
	class CombatQueries
	{
	public:
		explicit CombatQueries(const UnitRegistry& units);

		static uint32_t getChebyshevDistance(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);

		std::vector<EntityId> getAdjacentEnemies(EntityId actorId) const;

		std::vector<EntityId> getEnemiesInRange(
			EntityId actorId,
			uint32_t minRange,
			uint32_t maxRange) const;

	private:
		const UnitRegistry& _units;
	};

}
