#pragma once

#include <Battle/Units/Components/Types.hpp>

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace sw::battle
{
	class UnitRegistry;
	class Map;
	class CellOccupancy;

	using EntityId = components::EntityId;

	/** @brief Движение юнита по маршу на карте. */
	class MarchMovement
	{
	public:
		MarchMovement(const UnitRegistry& units, const Map& map, const CellOccupancy& occupancy);

		bool canMoveTowardMarch(EntityId entityId) const;

		std::optional<std::pair<uint32_t, uint32_t>> getNextStepToward(
			EntityId entityId,
			uint32_t targetX,
			uint32_t targetY) const;

		static std::vector<std::pair<uint32_t, uint32_t>> buildMarchStepCandidates(
			uint32_t x,
			uint32_t y,
			uint32_t targetX,
			uint32_t targetY);

	private:
		const UnitRegistry&  _units;
		const Map&           _map;
		const CellOccupancy& _occupancy;
	};

}
