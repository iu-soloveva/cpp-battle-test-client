#include "Battle.hpp"

#include <Battle/Units/Components/Types.hpp>
#include <Battle/Logging/EventLogVisitor.hpp>

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>

#include <stdexcept>

namespace sw::battle
{
	Battle::Battle(EventLog& eventLog)
		: _eventLog(eventLog)
	{
		registerCommands();
	}

	void Battle::loadScenario(std::istream& stream)
	{
		_state.reset();
		_parser.parse(stream);
	}

	void Battle::run()
	{
		if (!_state.getMap().hasMap())
		{
			throw std::runtime_error("Scenario must start with CREATE_MAP");
		}

		uint64_t tick = kSetupTick + 1;
		while (!_state.isFinished())
		{
			logging::logEvents(_eventLog, tick, _state.performNextStep());
			++tick;
		}
	}

	void Battle::onCreateMap(const io::CreateMap& command)
	{
		_state.createMap(command.width, command.height);
		logging::logEvent(_eventLog, kSetupTick, commands::MapCreatedEvent{command.width, command.height});
	}

	void Battle::onMarch(const io::March& command)
	{
		_state.startMarch(command.unitId, command.targetX, command.targetY);
		const components::Position* position =
			_state.getUnits().tryGetComponent<components::Position>(command.unitId);
		logging::logEvent(
			_eventLog,
			kSetupTick,
			commands::MarchStartedEvent{
				command.unitId,
				position->x,
				position->y,
				command.targetX,
				command.targetY});
	}

	void Battle::registerCommands()
	{
		_parser
			.add<io::CreateMap>([this](io::CreateMap command) { onCreateMap(command); })
			.add<io::SpawnSwordsman>(
				[this](io::SpawnSwordsman command) { onSpawn(command, "Swordsman"); })
			.add<io::SpawnHunter>([this](io::SpawnHunter command) { onSpawn(command, "Hunter"); })
			.add<io::March>([this](io::March command) { onMarch(command); });
	}
}
