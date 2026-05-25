#pragma once

#include <Battle/BattleState.hpp>
#include <Battle/Commands/CommandResult.hpp>
#include <Battle/Commands/ICommand.hpp>

#include <memory>

namespace sw::battle
{
	class IUnitBehavior
	{
	public:
		virtual ~IUnitBehavior() = default;

		virtual std::unique_ptr<commands::ICommand> selectCommand(
			const BattleState& state,
			EntityId entityId) const = 0;

		bool canAct(const BattleState& state, EntityId entityId) const
		{
			return selectCommand(state, entityId) != nullptr;
		}

		commands::CommandResult act(BattleState& state, EntityId entityId) const
		{
			const std::unique_ptr<commands::ICommand> command = selectCommand(state, entityId);
			if (command == nullptr)
			{
				return {};
			}

			return command->execute(state, entityId);
		}
	};
}
