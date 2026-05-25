#include "HunterBehavior.hpp"

#include <Battle/Commands/MeleeAttackCommand.hpp>
#include <Battle/Commands/MoveMarchCommand.hpp>
#include <Battle/Commands/RangedAttackCommand.hpp>

namespace sw::battle
{
	std::unique_ptr<commands::ICommand> HunterBehavior::selectCommand(
		const BattleState& state,
		EntityId entityId) const
	{
		const UnitRegistry& units = state.getUnits();

		if (!state.getQueries().getCombat().getAdjacentEnemies(entityId).empty())
		{
			const components::MeleeAttack* melee = units.tryGetComponent<components::MeleeAttack>(entityId);
			if (melee != nullptr)
			{
				return std::make_unique<commands::MeleeAttackCommand>(melee->damage);
			}
		}

		const components::RangedAttack* ranged = units.tryGetComponent<components::RangedAttack>(entityId);
		if (ranged != nullptr
			&& state.getQueries().getOccupancy().getAdjacentOccupants(entityId).empty()
			&& !state.getQueries()
					.getCombat()
					.getEnemiesInRange(entityId, ranged->minRange, ranged->maxRange)
					.empty())
		{
			return std::make_unique<commands::RangedAttackCommand>(
				ranged->minRange,
				ranged->maxRange,
				ranged->damage);
		}

		if (state.getQueries().getMarch().canMoveTowardMarch(entityId))
		{
			return std::make_unique<commands::MoveMarchCommand>();
		}

		return nullptr;
	}
}
