#include "CombatQueries.hpp"

#include <Battle/Units/UnitRegistry.hpp>

#include <algorithm>

namespace sw::battle
{
	CombatQueries::CombatQueries(const UnitRegistry& units)
		: _units(units)
	{
	}

	uint32_t CombatQueries::getChebyshevDistance(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
	{
		const uint32_t dx = x1 > x2 ? x1 - x2 : x2 - x1;
		const uint32_t dy = y1 > y2 ? y1 - y2 : y2 - y1;
		return std::max(dx, dy);
	}

	std::vector<EntityId> CombatQueries::getAdjacentEnemies(EntityId actorId) const
	{
		std::vector<EntityId> enemies;
		const components::Position* actorPos = _units.tryGetComponent<components::Position>(actorId);
		if (actorPos == nullptr)
		{
			return enemies;
		}

		for (EntityId otherId : _units.getSpawnOrder())
		{
			if (otherId == actorId || !_units.isUnitActive(otherId)
				|| !_units.hasTrait(otherId, components::UnitTrait::CanBeMeleeAttacked))
			{
				continue;
			}

			const components::Position* otherPos = _units.tryGetComponent<components::Position>(otherId);
			if (otherPos == nullptr)
			{
				continue;
			}

			if (getChebyshevDistance(actorPos->x, actorPos->y, otherPos->x, otherPos->y) == 1)
			{
				enemies.push_back(otherId);
			}
		}

		return enemies;
	}

	std::vector<EntityId> CombatQueries::getEnemiesInRange(
		EntityId actorId,
		uint32_t minRange,
		uint32_t maxRange) const
	{
		std::vector<EntityId> enemies;
		const components::Position* actorPos = _units.tryGetComponent<components::Position>(actorId);
		if (actorPos == nullptr)
		{
			return enemies;
		}

		for (EntityId otherId : _units.getSpawnOrder())
		{
			if (otherId == actorId || !_units.isUnitActive(otherId)
				|| !_units.hasTrait(otherId, components::UnitTrait::CanBeRangedAttacked))
			{
				continue;
			}

			const components::Position* otherPos = _units.tryGetComponent<components::Position>(otherId);
			if (otherPos == nullptr)
			{
				continue;
			}

			const uint32_t distance =
				getChebyshevDistance(actorPos->x, actorPos->y, otherPos->x, otherPos->y);
			if (distance >= minRange && distance <= maxRange)
			{
				enemies.push_back(otherId);
			}
		}

		return enemies;
	}
}
