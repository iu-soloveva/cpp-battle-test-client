#pragma once

#include <Battle/Commands/CommandResult.hpp>

#include <IO/Events/UnitSpawned.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/System/EventLog.hpp>

#include <cstdint>
#include <vector>

namespace sw::battle::logging
{
	/** @brief Преобразует domain CommandEvent в io-события и пишет их в EventLog. */
	struct EventLogVisitor
	{
		EventLog& eventLog;
		uint64_t const tick;

		void operator()(const commands::AttackEvent& event) const
		{
			eventLog.log(
				tick,
				io::UnitAttacked{
					event.attackerUnitId,
					event.targetUnitId,
					event.damage,
					event.targetHp});
		}

		void operator()(const commands::UnitDiedEvent& event) const
		{
			eventLog.log(tick, io::UnitDied{event.unitId});
		}

		void operator()(const commands::MovedEvent& event) const
		{
			eventLog.log(tick, io::UnitMoved{event.unitId, event.x, event.y});
		}

		void operator()(const commands::MarchEndedEvent& event) const
		{
			eventLog.log(tick, io::MarchEnded{event.unitId, event.x, event.y});
		}

		void operator()(const commands::MapCreatedEvent& event) const
		{
			eventLog.log(tick, io::MapCreated{event.width, event.height});
		}

		void operator()(const commands::UnitSpawnedEvent& event) const
		{
			eventLog.log(
				tick,
				io::UnitSpawned{
					.unitType = event.unitType,
					.unitId = event.unitId,
					.x = event.x,
					.y = event.y});
		}

		void operator()(const commands::MarchStartedEvent& event) const
		{
			eventLog.log(
				tick,
				io::MarchStarted{
					event.unitId,
					event.x,
					event.y,
					event.targetX,
					event.targetY});
		}
	};

	inline void logEvent(
		EventLog& eventLog,
		uint64_t const tick,
		const commands::CommandEvent& event)
	{
		std::visit(EventLogVisitor{eventLog, tick}, event);
	}

	inline void logEvents(
		EventLog& eventLog,
		uint64_t const tick,
		const std::vector<commands::CommandEvent>& events)
	{
		for (const commands::CommandEvent& event : events)
		{
			logEvent(eventLog, tick, event);
		}
	}
}
