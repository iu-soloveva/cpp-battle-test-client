#pragma once

#include "ICommand.hpp"

namespace sw::battle::commands
{
	class RangedAttackCommand final : public ICommand
	{
	public:
		RangedAttackCommand(uint32_t minRange, uint32_t maxRange, uint32_t damage);

		CommandResult execute(BattleState& state, EntityId entityId) const override;

	private:
		uint32_t _minRange{};
		uint32_t _maxRange{};
		uint32_t _damage{};
	};
}
