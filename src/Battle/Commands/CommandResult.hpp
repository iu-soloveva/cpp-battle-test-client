#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace sw::battle::commands
{
	/** @brief Результат атаки для логирования UNIT_ATTACKED. */
	struct AttackEvent
	{
		uint32_t attackerUnitId{};
		uint32_t targetUnitId{};
		uint32_t damage{};
		uint32_t targetHp{};
	};

	/** @brief Смерть юнита для логирования UNIT_DIED. */
	struct UnitDiedEvent
	{
		uint32_t unitId{};
	};

	/** @brief Перемещение для логирования UNIT_MOVED. */
	struct MovedEvent
	{
		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};
	};

	/** @brief Начало march для логирования MARCH_STARTED. */
	struct MarchStartedEvent
	{
		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};
		uint32_t targetX{};
		uint32_t targetY{};
	};

	/** @brief Завершение march для логирования MARCH_ENDED. */
	struct MarchEndedEvent
	{
		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};
	};

	/** @brief Создание карты для логирования MAP_CREATED. */
	struct MapCreatedEvent
	{
		uint32_t width{};
		uint32_t height{};
	};

	/** @brief Спавн юнита для логирования UNIT_SPAWNED. */
	struct UnitSpawnedEvent
	{
		std::string unitType;
		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};
	};

	/** @brief Одно доменное событие, порождённое командой. */
	using CommandEvent = std::variant<
		MapCreatedEvent,
		UnitSpawnedEvent,
		AttackEvent,
		UnitDiedEvent,
		MovedEvent,
		MarchStartedEvent,
		MarchEndedEvent>;

	/** @brief Результат выполнения команды. */
	struct CommandResult
	{
		std::vector<CommandEvent> events;
		bool performed{false};

		void add(CommandEvent event)
		{
			performed = true;
			events.push_back(std::move(event));
		}
	};
}
