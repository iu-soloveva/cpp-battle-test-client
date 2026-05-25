#pragma once

#include <cstdint>

namespace sw::battle::components
{
	using EntityId = uint32_t;

	enum class UnitType
	{
		Unknown,
		Swordsman,
		Hunter,
	};

	struct Position
	{
		uint32_t x{};
		uint32_t y{};
	};

	struct Health
	{
		uint32_t hp{};
	};

	struct MeleeAttack
	{
		uint32_t damage{};
	};

	struct RangedAttack
	{
		uint32_t minRange{};
		uint32_t maxRange{};
		uint32_t damage{};
	};

	struct March
	{
		uint32_t targetX{};
		uint32_t targetY{};
	};

	struct UnitIdentity
	{
		UnitType type{UnitType::Unknown};
	};

	enum class UnitTrait : uint32_t
	{
		OccupiesCell = 1u << 0,
		CanBeMeleeAttacked = 1u << 1,
		CanBeRangedAttacked = 1u << 2,
	};

	struct UnitTraits
	{
		uint32_t flags{};
	};

	constexpr uint32_t kDefaultCombatUnitTraits =
		static_cast<uint32_t>(UnitTrait::OccupiesCell)
		| static_cast<uint32_t>(UnitTrait::CanBeMeleeAttacked)
		| static_cast<uint32_t>(UnitTrait::CanBeRangedAttacked);
}
