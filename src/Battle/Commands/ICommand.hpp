#pragma once

#include <Battle/BattleState.hpp>
#include <Battle/Commands/CommandResult.hpp>

namespace sw::battle::commands
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;

		virtual CommandResult execute(BattleState& state, EntityId entityId) const = 0;
	};
}
