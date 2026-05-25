#include "RangedAttackCommand.hpp"

#include <Battle/Commands/Internal/Mechanics.hpp>

namespace sw::battle::commands
{
	RangedAttackCommand::RangedAttackCommand(uint32_t minRange, uint32_t maxRange, uint32_t damage)
		: _minRange(minRange)
		, _maxRange(maxRange)
		, _damage(damage)
	{
	}

	CommandResult RangedAttackCommand::execute(BattleState& state, EntityId entityId) const
	{
		return internal::tryRangedAttack(state, entityId, _minRange, _maxRange, _damage);
	}
}
