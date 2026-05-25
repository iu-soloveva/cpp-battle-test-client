#include "UnitFactory.hpp"

namespace sw::battle::units
{
	namespace
	{
		UnitDefinition::UnitSpawnParams convertToUnitSpawnParams(const io::SpawnSwordsman& command)
		{
			return UnitDefinition::UnitSpawnParams{
				.unitId = command.unitId,
				.position = {command.x, command.y},
				.hp = command.hp,
				.strength = command.strength,
			};
		}

		UnitDefinition::UnitSpawnParams convertToUnitSpawnParams(const io::SpawnHunter& command)
		{
			return UnitDefinition::UnitSpawnParams{
				.unitId = command.unitId,
				.position = {command.x, command.y},
				.hp = command.hp,
				.strength = command.strength,
				.agility = command.agility,
				.range = command.range,
			};
		}
	}

	void UnitFactory::spawn(
		BattleState& state,
		components::UnitType type,
		const UnitDefinition::UnitSpawnParams& params)
	{
		state.getQueries().getOccupancy().validateSpawnPosition(params.position);
		state.getUnits().registerUnit(params.unitId);
		UnitDefinitionRegistry::get(type).apply(state.getUnits(), params.unitId, params);
	}

	void UnitFactory::spawn(BattleState& state, const io::SpawnSwordsman& command)
	{
		spawn(state, components::UnitType::Swordsman, convertToUnitSpawnParams(command));
	}

	void UnitFactory::spawn(BattleState& state, const io::SpawnHunter& command)
	{
		spawn(state, components::UnitType::Hunter, convertToUnitSpawnParams(command));
	}
}
