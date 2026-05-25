#pragma once

#include <Battle/BattleState.hpp>
#include <Battle/Logging/EventLogVisitor.hpp>
#include <Battle/Units/UnitFactory.hpp>

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <istream>

#include <Battle/Commands/CommandResult.hpp>

namespace sw::battle
{
	/** @brief Оркестратор пошаговой битвы: сценарий, тики, логирование событий. */
	class Battle
	{
	public:
		explicit Battle(EventLog& eventLog);

		void loadScenario(std::istream& stream);

		void run();

	private:
		static constexpr uint64_t kSetupTick = 1;

		void onCreateMap(const io::CreateMap& command);
		void onMarch(const io::March& command);

		template <typename SpawnCommand>
		void onSpawn(const SpawnCommand& command, const char* unitTypeName)
		{
			units::UnitFactory::spawn(_state, command);
			logging::logEvent(
				_eventLog,
				kSetupTick,
				commands::UnitSpawnedEvent{
					.unitType = unitTypeName,
					.unitId = command.unitId,
					.x = command.x,
					.y = command.y});
		}

		void registerCommands();

	private:
		BattleState       _state;
		io::CommandParser _parser;
		EventLog&         _eventLog;
	};
}
