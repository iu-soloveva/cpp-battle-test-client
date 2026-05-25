#pragma once

#include <Battle/Units/UnitRegistry.hpp>
#include <Battle/Units/Components/Types.hpp>

namespace sw::battle::units
{
	/** @brief Статическое описание набора компонентов юнита. */
	struct UnitDefinition
	{
		/** @brief Параметры для спавна юнита. */
		struct UnitSpawnParams
		{
			components::EntityId unitId{};
			components::Position position{};
			uint32_t hp{};
			uint32_t strength{};
			uint32_t agility{};
			uint32_t range{};
		};

		components::UnitType type{components::UnitType::Unknown};
		uint32_t traits{};
		bool hasHealth{};
		bool hasMeleeAttack{};
		bool hasRangedAttack{};
		uint32_t rangedMinRange{};

		void apply(UnitRegistry& registry, components::EntityId id, const UnitSpawnParams& params) const;
	};

	/** @brief Реестр описаний юнитов по типу. */
	class UnitDefinitionRegistry
	{
	public:
		static const UnitDefinition& get(components::UnitType type);
	};
}
