#pragma once

#include <Battle/Units/Components/Storage.hpp>
#include <Battle/Units/Components/Types.hpp>

#include <cstdint>
#include <vector>

namespace sw::battle
{
	using EntityId = components::EntityId;
	using UnitType = components::UnitType;

	/** @brief Реестр юнитов на поле и их компонентов. */
	class UnitRegistry
	{
	public:
		void clear();

		void registerUnit(EntityId id);
		bool hasUnit(EntityId id) const;
		size_t getAliveUnitCount() const;
		bool isUnitActive(EntityId id) const;
		UnitType getUnitType(EntityId id) const;

		void startMarch(EntityId id, uint32_t targetX, uint32_t targetY);

		void setPendingRemoval(EntityId id);
		bool isPendingRemoval(EntityId id) const;

		bool hasTrait(EntityId id, components::UnitTrait trait) const;

	public:
		const std::vector<EntityId>& getSpawnOrder() const { return _spawnOrder; }

	public:
		template <typename T>
		bool hasComponent(EntityId id) const
		{
			return _components.has<T>(id);
		}

		template <typename T>
		const T* tryGetComponent(EntityId id) const
		{
			return _components.tryGet<T>(id);
		}

		template <typename T>
		T* tryGetComponent(EntityId id)
		{
			return _components.tryGet<T>(id);
		}

		template <typename T>
		void setComponent(EntityId id, T component)
		{
			_components.emplace(id, std::move(component));
		}

		template <typename T>
		void removeComponent(EntityId id)
		{
			_components.remove<T>(id);
		}

	private:
		components::ComponentStorage _components;
		std::vector<EntityId>        _spawnOrder;
	};
}
