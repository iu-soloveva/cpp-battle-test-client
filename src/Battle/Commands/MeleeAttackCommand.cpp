#include "MeleeAttackCommand.hpp"

#include <Battle/Commands/Internal/Mechanics.hpp>

namespace sw::battle::commands
{
	MeleeAttackCommand::MeleeAttackCommand(uint32_t damage)
		: _damage(damage)
	{
	}

	CommandResult MeleeAttackCommand::execute(BattleState& state, EntityId entityId) const
	{
		return internal::tryMeleeAttack(state, entityId, _damage);
	}
}
