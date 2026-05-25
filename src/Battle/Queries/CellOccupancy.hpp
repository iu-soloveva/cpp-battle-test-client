#pragma once

#include <Battle/Units/Components/Types.hpp>

#include <cstdint>
#include <optional>
#include <vector>

namespace sw::battle
{
	class UnitRegistry;
	class Map;

	using EntityId = components::EntityId;

	/** @brief Запросы о занятости клеток юнитами. */
	class CellOccupancy
	{
	public:
		CellOccupancy(const UnitRegistry& units, const Map& map);

		void validateSpawnPosition(components::Position position) const;

		bool isOccupied(uint32_t x, uint32_t y, EntityId exceptEntityId = 0) const;

		std::optional<EntityId> getEntityAt(uint32_t x, uint32_t y) const;

		std::vector<EntityId> getAdjacentOccupants(EntityId entityId) const;

	private:
		const UnitRegistry& _units;
		const Map&          _map;
	};

}
