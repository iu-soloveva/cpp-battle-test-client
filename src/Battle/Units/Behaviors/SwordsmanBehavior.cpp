#include "SwordsmanBehavior.hpp"

#include <Battle/Commands/MeleeAttackCommand.hpp>
#include <Battle/Commands/MoveMarchCommand.hpp>

namespace sw::battle
{
	std::unique_ptr<commands::ICommand> SwordsmanBehavior::selectCommand(
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

		if (state.getQueries().getMarch().canMoveTowardMarch(entityId))
		{
			return std::make_unique<commands::MoveMarchCommand>();
		}

		return nullptr;
	}
}
