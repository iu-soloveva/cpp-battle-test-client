#include "UnitRegistry.hpp"

#include <stdexcept>

namespace sw::battle
{
	void UnitRegistry::clear()
	{
		_components.clear();
		_spawnOrder.clear();
	}

	void UnitRegistry::registerUnit(EntityId id)
	{
		if (hasUnit(id))
		{
			throw std::runtime_error("Duplicate unit id: " + std::to_string(id));
		}

		_spawnOrder.push_back(id);
	}

	bool UnitRegistry::hasUnit(EntityId id) const
	{
		return _components.has<components::UnitIdentity>(id);
	}

	size_t UnitRegistry::getAliveUnitCount() const
	{
		size_t count = 0;
		for (EntityId id : _spawnOrder)
		{
			if (isUnitActive(id))
			{
				++count;
			}
		}
		return count;
	}

	bool UnitRegistry::isUnitActive(EntityId id) const
	{
		if (!hasUnit(id) || _components.isPendingRemoval(id))
		{
			return false;
		}

		const components::Health* health = _components.tryGet<components::Health>(id);
		if (health == nullptr)
		{
			return true;
		}

		return health->hp > 0;
	}

	UnitType UnitRegistry::getUnitType(EntityId id) const
	{
		const components::UnitIdentity* identity = _components.tryGet<components::UnitIdentity>(id);
		if (identity == nullptr)
		{
			throw std::runtime_error("Unknown unit id: " + std::to_string(id));
		}

		return identity->type;
	}

	void UnitRegistry::startMarch(EntityId id, uint32_t targetX, uint32_t targetY)
	{
		if (!hasUnit(id))
		{
			throw std::runtime_error("Unknown unit for MARCH: " + std::to_string(id));
		}

		if (components::March* march = _components.tryGet<components::March>(id))
		{
			march->targetX = targetX;
			march->targetY = targetY;
			return;
		}

		_components.emplace(id, components::March{targetX, targetY});
	}

	void UnitRegistry::setPendingRemoval(EntityId id)
	{
		_components.markPendingRemoval(id);
	}

	bool UnitRegistry::isPendingRemoval(EntityId id) const
	{
		return _components.isPendingRemoval(id);
	}

	bool UnitRegistry::hasTrait(EntityId id, components::UnitTrait trait) const
	{
		const components::UnitTraits* traits = _components.tryGet<components::UnitTraits>(id);
		if (traits == nullptr)
		{
			return false;
		}

		return (traits->flags & static_cast<uint32_t>(trait)) != 0;
	}
}
