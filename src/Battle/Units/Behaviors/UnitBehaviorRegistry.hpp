#pragma once

#include "IUnitBehavior.hpp"

namespace sw::battle
{
	/** @brief Реестр поведений юнитов по типу. */
	class UnitBehaviorRegistry
	{
	public:
		static const IUnitBehavior& get(UnitType type);
	};
}
