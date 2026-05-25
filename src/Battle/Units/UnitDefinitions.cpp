#include "UnitDefinitions.hpp"

#include <stdexcept>
#include <unordered_map>

namespace sw::battle::units
{
	void UnitDefinition::apply(
		UnitRegistry& registry,
		components::EntityId id,
		const UnitSpawnParams& params) const
	{
		registry.setComponent(id, components::UnitIdentity{type});
		registry.setComponent(id, params.position);

		if (hasHealth)
		{
			registry.setComponent(id, components::Health{params.hp});
		}

		if (hasMeleeAttack)
		{
			registry.setComponent(id, components::MeleeAttack{params.strength});
		}

		if (hasRangedAttack)
		{
			registry.setComponent(
				id,
				components::RangedAttack{rangedMinRange, params.range, params.agility});
		}

		registry.setComponent(id, components::UnitTraits{traits});
	}

	namespace
	{
		using DefinitionMap = std::unordered_map<components::UnitType, UnitDefinition>;

		const DefinitionMap& definitions()
		{
			static const DefinitionMap map{
				{components::UnitType::Swordsman,
				 UnitDefinition{
					 .type = components::UnitType::Swordsman,
					 .traits = components::kDefaultCombatUnitTraits,
					 .hasHealth = true,
					 .hasMeleeAttack = true,
					 .hasRangedAttack = false,
				 }},
				{components::UnitType::Hunter,
				 UnitDefinition{
					 .type = components::UnitType::Hunter,
					 .traits = components::kDefaultCombatUnitTraits,
					 .hasHealth = true,
					 .hasMeleeAttack = true,
					 .hasRangedAttack = true,
					 .rangedMinRange = 2,
				 }},
			};

			return map;
		}
	}

	const UnitDefinition& UnitDefinitionRegistry::get(components::UnitType type)
	{
		const DefinitionMap& map = definitions();
		const auto it = map.find(type);
		if (it == map.end())
		{
			throw std::runtime_error("Unknown unit type definition");
		}

		return it->second;
	}
}
