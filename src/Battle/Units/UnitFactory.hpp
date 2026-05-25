#pragma once

#include <Battle/BattleState.hpp>
#include <Battle/Units/UnitDefinitions.hpp>

#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>

namespace sw::battle::units
{
	class UnitFactory
	{
	public:
		static void spawn(
			BattleState& state,
			components::UnitType type,
			const UnitDefinition::UnitSpawnParams& params);

		static void spawn(BattleState& state, const io::SpawnSwordsman& command);

		static void spawn(BattleState& state, const io::SpawnHunter& command);
	};
}
