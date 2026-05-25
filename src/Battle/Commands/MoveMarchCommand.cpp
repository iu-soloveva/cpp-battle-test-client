#include "MoveMarchCommand.hpp"

#include <Battle/Commands/Internal/Mechanics.hpp>

namespace sw::battle::commands
{
	CommandResult MoveMarchCommand::execute(BattleState& state, EntityId entityId) const
	{
		return internal::tryMoveTowardMarch(state, entityId);
	}
}
