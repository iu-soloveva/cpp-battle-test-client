#include "CellOccupancy.hpp"

#include <Battle/Units/UnitRegistry.hpp>
#include <Battle/Map.hpp>

#include <optional>
#include <stdexcept>

namespace sw::battle
{
	CellOccupancy::CellOccupancy(const UnitRegistry& units, const Map& map)
		: _units(units)
		, _map(map)
	{
	}

	void CellOccupancy::validateSpawnPosition(components::Position position) const
	{
		if (!_map.isInside(position.x, position.y))
		{
			throw std::runtime_error("Unit spawn position is outside the map");
		}
		if (isOccupied(position.x, position.y))
		{
			throw std::runtime_error("Unit spawn position is already occupied");
		}
	}

	bool CellOccupancy::isOccupied(uint32_t x, uint32_t y, EntityId exceptEntityId) const
	{
		for (EntityId id : _units.getSpawnOrder())
		{
			if (id == exceptEntityId || !_units.isUnitActive(id)
				|| !_units.hasTrait(id, components::UnitTrait::OccupiesCell))
			{
				continue;
			}

			const components::Position* position = _units.tryGetComponent<components::Position>(id);
			if (position != nullptr && position->x == x && position->y == y)
			{
				return true;
			}
		}

		return false;
	}

	std::optional<EntityId> CellOccupancy::getEntityAt(uint32_t x, uint32_t y) const
	{
		for (EntityId id : _units.getSpawnOrder())
		{
			if (!_units.isUnitActive(id)
				|| !_units.hasTrait(id, components::UnitTrait::OccupiesCell))
			{
				continue;
			}

			const components::Position* position = _units.tryGetComponent<components::Position>(id);
			if (position != nullptr && position->x == x && position->y == y)
			{
				return id;
			}
		}

		return std::nullopt;
	}

	std::vector<EntityId> CellOccupancy::getAdjacentOccupants(EntityId entityId) const
	{
		std::vector<EntityId> occupants;
		const components::Position* position = _units.tryGetComponent<components::Position>(entityId);
		if (position == nullptr)
		{
			return occupants;
		}

		for (int32_t dx = -1; dx <= 1; ++dx)
		{
			for (int32_t dy = -1; dy <= 1; ++dy)
			{
				if (dx == 0 && dy == 0)
				{
					continue;
				}

				const int32_t nx = static_cast<int32_t>(position->x) + dx;
				const int32_t ny = static_cast<int32_t>(position->y) + dy;
				if (nx < 0 || ny < 0)
				{
					continue;
				}

				const std::optional<EntityId> occupant =
					getEntityAt(static_cast<uint32_t>(nx), static_cast<uint32_t>(ny));
				if (occupant.has_value())
				{
					occupants.push_back(*occupant);
				}
			}
		}

		return occupants;
	}
}
