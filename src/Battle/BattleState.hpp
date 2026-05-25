#pragma once

#include <Battle/Units/UnitRegistry.hpp>
#include <Battle/Map.hpp>
#include <Battle/Commands/CommandResult.hpp>
#include <Battle/Queries/BattleQueries.hpp>

#include <vector>

namespace sw::battle
{
	/** @brief Состояние битвы: юниты, карта, запросы и правила хода. */
	class BattleState
	{
	public:
		BattleState();

		void reset();

		void createMap(uint32_t width, uint32_t height);

		void startMarch(EntityId unitId, uint32_t targetX, uint32_t targetY);

		std::vector<commands::CommandEvent> performNextStep();

		bool canAnyUnitAct() const;

		bool isFinished() const;

	public:
		BattleQueries& getQueries() { return _queries; }
		const BattleQueries& getQueries() const { return _queries; }

		const Map& getMap() const { return _map; }

		UnitRegistry& getUnits() { return _units; }
		const UnitRegistry& getUnits() const { return _units; }

	private:
		void removeDeadUnits(std::vector<commands::CommandEvent>& events);

		void processUnitActions(std::vector<commands::CommandEvent>& events);

	private:
		UnitRegistry  _units;
		Map           _map;
		BattleQueries _queries;
	};
}
