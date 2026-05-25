#pragma once

#include "ICommand.hpp"

namespace sw::battle::commands
{
	class MeleeAttackCommand final : public ICommand
	{
	public:
		explicit MeleeAttackCommand(uint32_t damage);

		CommandResult execute(BattleState& state, EntityId entityId) const override;

	private:
		uint32_t _damage{};
	};
}
