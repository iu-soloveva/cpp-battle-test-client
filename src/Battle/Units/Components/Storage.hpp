#pragma once

#include <Battle/Units/Components/Types.hpp>

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace sw::battle::components
{
	class ComponentStorage
	{
	public:
		void clear();

		bool isPendingRemoval(EntityId id) const;
		void markPendingRemoval(EntityId id);

		template <typename T>
		bool has(EntityId id) const;

		template <typename T>
		const T* tryGet(EntityId id) const;

		template <typename T>
		T* tryGet(EntityId id);

		template <typename T>
		T& get(EntityId id);

		template <typename T>
		void emplace(EntityId id, T component);

		template <typename T>
		void remove(EntityId id);

	private:
		std::unordered_map<EntityId, UnitIdentity> _identity;
		std::unordered_map<EntityId, Position> _positions;
		std::unordered_map<EntityId, Health> _health;
		std::unordered_map<EntityId, MeleeAttack> _melee;
		std::unordered_map<EntityId, RangedAttack> _ranged;
		std::unordered_map<EntityId, March> _march;
		std::unordered_map<EntityId, UnitTraits> _traits;
		std::unordered_set<EntityId> _pendingRemoval;
	};

	inline void ComponentStorage::clear()
	{
		_identity.clear();
		_positions.clear();
		_health.clear();
		_melee.clear();
		_ranged.clear();
		_march.clear();
		_traits.clear();
		_pendingRemoval.clear();
	}

	inline bool ComponentStorage::isPendingRemoval(EntityId id) const
	{
		return _pendingRemoval.contains(id);
	}

	inline void ComponentStorage::markPendingRemoval(EntityId id)
	{
		_pendingRemoval.insert(id);
	}

	template <>
	inline bool ComponentStorage::has<UnitIdentity>(EntityId id) const
	{
		return _identity.contains(id);
	}

	template <>
	inline bool ComponentStorage::has<Position>(EntityId id) const
	{
		return _positions.contains(id);
	}

	template <>
	inline bool ComponentStorage::has<Health>(EntityId id) const
	{
		return _health.contains(id);
	}

	template <>
	inline bool ComponentStorage::has<MeleeAttack>(EntityId id) const
	{
		return _melee.contains(id);
	}

	template <>
	inline bool ComponentStorage::has<RangedAttack>(EntityId id) const
	{
		return _ranged.contains(id);
	}

	template <>
	inline bool ComponentStorage::has<March>(EntityId id) const
	{
		return _march.contains(id);
	}

	template <>
	inline bool ComponentStorage::has<UnitTraits>(EntityId id) const
	{
		return _traits.contains(id);
	}

	template <>
	inline const UnitIdentity* ComponentStorage::tryGet<UnitIdentity>(EntityId id) const
	{
		const auto it = _identity.find(id);
		return it != _identity.end() ? &it->second : nullptr;
	}

	template <>
	inline const Position* ComponentStorage::tryGet<Position>(EntityId id) const
	{
		const auto it = _positions.find(id);
		return it != _positions.end() ? &it->second : nullptr;
	}

	template <>
	inline const Health* ComponentStorage::tryGet<Health>(EntityId id) const
	{
		const auto it = _health.find(id);
		return it != _health.end() ? &it->second : nullptr;
	}

	template <>
	inline const MeleeAttack* ComponentStorage::tryGet<MeleeAttack>(EntityId id) const
	{
		const auto it = _melee.find(id);
		return it != _melee.end() ? &it->second : nullptr;
	}

	template <>
	inline const RangedAttack* ComponentStorage::tryGet<RangedAttack>(EntityId id) const
	{
		const auto it = _ranged.find(id);
		return it != _ranged.end() ? &it->second : nullptr;
	}

	template <>
	inline const March* ComponentStorage::tryGet<March>(EntityId id) const
	{
		const auto it = _march.find(id);
		return it != _march.end() ? &it->second : nullptr;
	}

	template <>
	inline const UnitTraits* ComponentStorage::tryGet<UnitTraits>(EntityId id) const
	{
		const auto it = _traits.find(id);
		return it != _traits.end() ? &it->second : nullptr;
	}

	template <>
	inline UnitIdentity* ComponentStorage::tryGet<UnitIdentity>(EntityId id)
	{
		auto it = _identity.find(id);
		return it != _identity.end() ? &it->second : nullptr;
	}

	template <>
	inline Position* ComponentStorage::tryGet<Position>(EntityId id)
	{
		auto it = _positions.find(id);
		return it != _positions.end() ? &it->second : nullptr;
	}

	template <>
	inline Health* ComponentStorage::tryGet<Health>(EntityId id)
	{
		auto it = _health.find(id);
		return it != _health.end() ? &it->second : nullptr;
	}

	template <>
	inline MeleeAttack* ComponentStorage::tryGet<MeleeAttack>(EntityId id)
	{
		auto it = _melee.find(id);
		return it != _melee.end() ? &it->second : nullptr;
	}

	template <>
	inline RangedAttack* ComponentStorage::tryGet<RangedAttack>(EntityId id)
	{
		auto it = _ranged.find(id);
		return it != _ranged.end() ? &it->second : nullptr;
	}

	template <>
	inline March* ComponentStorage::tryGet<March>(EntityId id)
	{
		auto it = _march.find(id);
		return it != _march.end() ? &it->second : nullptr;
	}

	template <>
	inline UnitTraits* ComponentStorage::tryGet<UnitTraits>(EntityId id)
	{
		auto it = _traits.find(id);
		return it != _traits.end() ? &it->second : nullptr;
	}

	template <>
	inline UnitIdentity& ComponentStorage::get<UnitIdentity>(EntityId id)
	{
		return _identity.at(id);
	}

	template <>
	inline Position& ComponentStorage::get<Position>(EntityId id)
	{
		return _positions.at(id);
	}

	template <>
	inline Health& ComponentStorage::get<Health>(EntityId id)
	{
		return _health.at(id);
	}

	template <>
	inline MeleeAttack& ComponentStorage::get<MeleeAttack>(EntityId id)
	{
		return _melee.at(id);
	}

	template <>
	inline RangedAttack& ComponentStorage::get<RangedAttack>(EntityId id)
	{
		return _ranged.at(id);
	}

	template <>
	inline March& ComponentStorage::get<March>(EntityId id)
	{
		return _march.at(id);
	}

	template <>
	inline UnitTraits& ComponentStorage::get<UnitTraits>(EntityId id)
	{
		return _traits.at(id);
	}

	template <>
	inline void ComponentStorage::emplace<UnitIdentity>(EntityId id, UnitIdentity component)
	{
		_identity.emplace(id, component);
	}

	template <>
	inline void ComponentStorage::emplace<Position>(EntityId id, Position component)
	{
		_positions.emplace(id, component);
	}

	template <>
	inline void ComponentStorage::emplace<Health>(EntityId id, Health component)
	{
		_health.emplace(id, component);
	}

	template <>
	inline void ComponentStorage::emplace<MeleeAttack>(EntityId id, MeleeAttack component)
	{
		_melee.emplace(id, component);
	}

	template <>
	inline void ComponentStorage::emplace<RangedAttack>(EntityId id, RangedAttack component)
	{
		_ranged.emplace(id, component);
	}

	template <>
	inline void ComponentStorage::emplace<March>(EntityId id, March component)
	{
		_march.emplace(id, component);
	}

	template <>
	inline void ComponentStorage::emplace<UnitTraits>(EntityId id, UnitTraits component)
	{
		_traits.emplace(id, component);
	}

	template <>
	inline void ComponentStorage::remove<UnitIdentity>(EntityId id)
	{
		_identity.erase(id);
	}

	template <>
	inline void ComponentStorage::remove<Position>(EntityId id)
	{
		_positions.erase(id);
	}

	template <>
	inline void ComponentStorage::remove<Health>(EntityId id)
	{
		_health.erase(id);
	}

	template <>
	inline void ComponentStorage::remove<MeleeAttack>(EntityId id)
	{
		_melee.erase(id);
	}

	template <>
	inline void ComponentStorage::remove<RangedAttack>(EntityId id)
	{
		_ranged.erase(id);
	}

	template <>
	inline void ComponentStorage::remove<March>(EntityId id)
	{
		_march.erase(id);
	}

	template <>
	inline void ComponentStorage::remove<UnitTraits>(EntityId id)
	{
		_traits.erase(id);
	}
}
