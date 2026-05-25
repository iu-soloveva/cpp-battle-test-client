#pragma once

#include "IUnitBehavior.hpp"

namespace sw::battle
{
	class SwordsmanBehavior final : public IUnitBehavior
	{
	public:
		std::unique_ptr<commands::ICommand> selectCommand(
			const BattleState& state,
			EntityId entityId) const override;
	};
}
