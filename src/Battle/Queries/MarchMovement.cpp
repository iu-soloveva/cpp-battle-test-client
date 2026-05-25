#include "MarchMovement.hpp"

#include <Battle/Queries/CellOccupancy.hpp>
#include <Battle/Units/UnitRegistry.hpp>
#include <Battle/Map.hpp>

namespace sw::battle
{
	namespace
	{
		int32_t stepToward(uint32_t from, uint32_t to)
		{
			if (from < to)
			{
				return 1;
			}
			if (from > to)
			{
				return -1;
			}
			return 0;
		}
	}

	MarchMovement::MarchMovement(
		const UnitRegistry& units,
		const Map& map,
		const CellOccupancy& occupancy)
		: _units(units)
		, _map(map)
		, _occupancy(occupancy)
	{
	}

	bool MarchMovement::canMoveTowardMarch(EntityId entityId) const
	{
		const components::March* march = _units.tryGetComponent<components::March>(entityId);
		const components::Position* position = _units.tryGetComponent<components::Position>(entityId);
		if (march == nullptr || position == nullptr)
		{
			return false;
		}

		if (position->x == march->targetX && position->y == march->targetY)
		{
			return true;
		}

		return getNextStepToward(entityId, march->targetX, march->targetY).has_value();
	}

	std::optional<std::pair<uint32_t, uint32_t>> MarchMovement::getNextStepToward(
		EntityId entityId,
		uint32_t targetX,
		uint32_t targetY) const
	{
		const components::Position* position = _units.tryGetComponent<components::Position>(entityId);
		if (position == nullptr)
		{
			return std::nullopt;
		}

		for (const auto& [nextX, nextY] :
			buildMarchStepCandidates(position->x, position->y, targetX, targetY))
		{
			if (_map.isInside(nextX, nextY) && !_occupancy.isOccupied(nextX, nextY, entityId))
			{
				return std::pair{nextX, nextY};
			}
		}

		return std::nullopt;
	}

	std::vector<std::pair<uint32_t, uint32_t>> MarchMovement::buildMarchStepCandidates(
		uint32_t x,
		uint32_t y,
		uint32_t targetX,
		uint32_t targetY)
	{
		const int32_t dx = stepToward(x, targetX);
		const int32_t dy = stepToward(y, targetY);

		std::vector<std::pair<uint32_t, uint32_t>> candidates;
		if (dx != 0 && dy != 0)
		{
			candidates.emplace_back(
				static_cast<uint32_t>(static_cast<int32_t>(x) + dx),
				static_cast<uint32_t>(static_cast<int32_t>(y) + dy));
		}
		if (dx != 0)
		{
			candidates.emplace_back(static_cast<uint32_t>(static_cast<int32_t>(x) + dx), y);
		}
		if (dy != 0)
		{
			candidates.emplace_back(x, static_cast<uint32_t>(static_cast<int32_t>(y) + dy));
		}

		return candidates;
	}
}
