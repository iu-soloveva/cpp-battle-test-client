#pragma once

#include "ICommand.hpp"

namespace sw::battle::commands
{
	class MoveMarchCommand final : public ICommand
	{
	public:
		CommandResult execute(BattleState& state, EntityId entityId) const override;
	};
}
