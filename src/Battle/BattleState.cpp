#include "BattleState.hpp"

#include <Battle/Units/Behaviors/UnitBehaviorRegistry.hpp>

#include <stdexcept>

namespace sw::battle
{
	BattleState::BattleState()
		: _queries(_units, _map)
	{
	}

	void BattleState::reset()
	{
		_units.clear();
		_map.reset();
	}

	void BattleState::createMap(uint32_t width, uint32_t height)
	{
		_map.create(width, height);
	}

	void BattleState::startMarch(EntityId unitId, uint32_t targetX, uint32_t targetY)
	{
		if (!_units.hasUnit(unitId))
		{
			throw std::runtime_error("Unknown unit for MARCH: " + std::to_string(unitId));
		}

		_units.startMarch(unitId, targetX, targetY);
	}

	namespace
	{
		void appendResult(std::vector<commands::CommandEvent>& events, commands::CommandResult result)
		{
			if (!result.performed)
			{
				return;
			}

			events.insert(
				events.end(),
				std::make_move_iterator(result.events.begin()),
				std::make_move_iterator(result.events.end()));
		}
	}

	std::vector<commands::CommandEvent> BattleState::performNextStep()
	{
		std::vector<commands::CommandEvent> events;
		removeDeadUnits(events);
		processUnitActions(events);
		return events;
	}

	void BattleState::removeDeadUnits(std::vector<commands::CommandEvent>& events)
	{
		for (EntityId entityId : _units.getSpawnOrder())
		{
			const components::Health* health = _units.tryGetComponent<components::Health>(entityId);
			if (health != nullptr && health->hp == 0 && !_units.isPendingRemoval(entityId))
			{
				_units.setPendingRemoval(entityId);
				events.push_back(commands::UnitDiedEvent{entityId});
			}
		}
	}

	void BattleState::processUnitActions(std::vector<commands::CommandEvent>& events)
	{
		for (EntityId entityId : _units.getSpawnOrder())
		{
			if (!_units.isUnitActive(entityId))
			{
				continue;
			}

			const commands::CommandResult result =
				UnitBehaviorRegistry::get(_units.getUnitType(entityId)).act(*this, entityId);
			appendResult(events, std::move(result));
		}
	}

	bool BattleState::canAnyUnitAct() const
	{
		for (EntityId entityId : _units.getSpawnOrder())
		{
			if (_units.isUnitActive(entityId)
				&& UnitBehaviorRegistry::get(_units.getUnitType(entityId)).canAct(*this, entityId))
			{
				return true;
			}
		}
		return false;
	}

	bool BattleState::isFinished() const
	{
		return _units.getAliveUnitCount() <= 1 || !canAnyUnitAct();
	}
}
