#include "UnitBehaviorRegistry.hpp"

#include "HunterBehavior.hpp"
#include "SwordsmanBehavior.hpp"

#include <functional>
#include <stdexcept>
#include <unordered_map>

namespace sw::battle
{
	namespace
	{
		using BehaviorMap = std::unordered_map<UnitType, std::reference_wrapper<const IUnitBehavior>>;

		const BehaviorMap& behaviors()
		{
			static const SwordsmanBehavior swordsman;
			static const HunterBehavior hunter;

			static const BehaviorMap map{
				{UnitType::Swordsman, std::cref(swordsman)},
				{UnitType::Hunter, std::cref(hunter)},
			};

			return map;
		}
	}

	const IUnitBehavior& UnitBehaviorRegistry::get(UnitType type)
	{
		const BehaviorMap& map = behaviors();
		const auto it = map.find(type);
		if (it == map.end())
		{
			throw std::runtime_error("Unknown unit type behavior");
		}

		return it->second.get();
	}
}
