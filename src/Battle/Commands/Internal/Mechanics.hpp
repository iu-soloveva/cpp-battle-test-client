#pragma once

#include <Battle/BattleState.hpp>
#include <Battle/Commands/CommandResult.hpp>

namespace sw::battle::commands::internal
{
	CommandResult tryMeleeAttack(BattleState& state, EntityId attackerId, uint32_t damage);

	CommandResult tryRangedAttack(
		BattleState& state,
		EntityId attackerId,
		uint32_t minRange,
		uint32_t maxRange,
		uint32_t damage);

	CommandResult tryMoveTowardMarch(BattleState& state, EntityId entityId);
}
