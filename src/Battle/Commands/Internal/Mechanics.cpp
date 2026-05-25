#include "Mechanics.hpp"

#include <random>
#include <utility>
#include <vector>

namespace sw::battle::commands::internal
{
	namespace
	{
		std::mt19937& selectionRng()
		{
			static std::mt19937 rng(std::random_device{}());
			return rng;
		}

		EntityId pickRandomTarget(const std::vector<EntityId>& candidates)
		{
			std::uniform_int_distribution<size_t> distribution(0, candidates.size() - 1);
			return candidates[distribution(selectionRng())];
		}

		CommandResult performAttack(
			UnitRegistry& registry,
			EntityId attackerId,
			EntityId targetId,
			uint32_t damage)
		{
			components::Health* targetHealth = registry.tryGetComponent<components::Health>(targetId);
			if (targetHealth == nullptr)
			{
				return {};
			}

			const uint32_t newHp = targetHealth->hp > damage ? targetHealth->hp - damage : 0;
			targetHealth->hp = newHp;

			CommandResult result;
			result.add(AttackEvent{attackerId, targetId, damage, newHp});

			if (newHp == 0)
			{
				registry.setPendingRemoval(targetId);
				result.add(UnitDiedEvent{targetId});
			}

			return result;
		}
	}

	CommandResult tryMeleeAttack(BattleState& state, EntityId attackerId, uint32_t damage)
	{
		std::vector<EntityId> enemies = state.getQueries().getCombat().getAdjacentEnemies(attackerId);
		if (enemies.empty())
		{
			return {};
		}

		const EntityId targetId = pickRandomTarget(enemies);
		return performAttack(state.getUnits(), attackerId, targetId, damage);
	}

	CommandResult tryRangedAttack(
		BattleState& state,
		EntityId attackerId,
		uint32_t minRange,
		uint32_t maxRange,
		uint32_t damage)
	{
		std::vector<EntityId> enemies =
			state.getQueries().getCombat().getEnemiesInRange(attackerId, minRange, maxRange);
		if (enemies.empty())
		{
			return {};
		}

		const EntityId targetId = pickRandomTarget(enemies);
		return performAttack(state.getUnits(), attackerId, targetId, damage);
	}

	CommandResult tryMoveTowardMarch(BattleState& state, EntityId entityId)
	{
		UnitRegistry& units = state.getUnits();
		components::March* march = units.tryGetComponent<components::March>(entityId);
		components::Position* position = units.tryGetComponent<components::Position>(entityId);
		if (march == nullptr || position == nullptr)
		{
			return {};
		}

		if (position->x == march->targetX && position->y == march->targetY)
		{
			CommandResult result;
			result.add(MarchEndedEvent{entityId, position->x, position->y});
			units.removeComponent<components::March>(entityId);
			return result;
		}

		const std::optional<std::pair<uint32_t, uint32_t>> nextStep =
			state.getQueries().getMarch().getNextStepToward(entityId, march->targetX, march->targetY);
		if (!nextStep.has_value())
		{
			return {};
		}

		position->x = nextStep->first;
		position->y = nextStep->second;

		CommandResult result;
		result.add(MovedEvent{entityId, position->x, position->y});

		if (position->x == march->targetX && position->y == march->targetY)
		{
			result.add(MarchEndedEvent{entityId, position->x, position->y});
			units.removeComponent<components::March>(entityId);
		}

		return result;
	}
}
