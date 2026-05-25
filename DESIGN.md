# Design decisions

Раздел для ревьюера: что сделано сознательно, какие узкие места найдены и почему они **не** реализованы.

## Архитектура

- `BattleState` — оркестрация тика: удаление мёртвых, ходы юнитов, условие конца боя.
- `UnitRegistry` + `ComponentStorage` — ECS-lite: компоненты по `EntityId`, порядок ходов через `_spawnOrder`.
- `BattleQueries` — фасад доменных запросов: `CellOccupancy`, `CombatQueries`, `MarchMovement`.
- `IUnitBehavior` + `ICommand` — Strategy/Command: behavior выбирает команду, команда выполняет механику.
- `UnitTrait` — расширяемость под будущих юнитов (ворон без `OccupiesCell`, мина без HP и т.д.).

## Схема классов и зависимостей

```mermaid
classDiagram
    direction TB

    class Battle {
        -BattleState _state
        -CommandParser _parser
        -EventLog& _eventLog
        +loadScenario()
        +run()
    }

    class BattleState {
        -UnitRegistry _units
        -Map _map
        -BattleQueries _queries
        +performNextStep()
        +isFinished()
    }

    class Map
    class UnitRegistry {
        -ComponentStorage _components
        -vector _spawnOrder
    }
    class ComponentStorage
    class BattleQueries {
        -CellOccupancy _occupancy
        -CombatQueries _combat
        -MarchMovement _march
    }
    class CellOccupancy
    class CombatQueries
    class MarchMovement

    class UnitFactory {
        +spawn()$
    }
    class UnitDefinitionRegistry {
        +get()$
    }
    class UnitDefinition

    class UnitBehaviorRegistry {
        +get()$
    }
    class IUnitBehavior {
        <<interface>>
        +selectCommand()
        +canAct()
        +act()
    }
    class SwordsmanBehavior
    class HunterBehavior

    class ICommand {
        <<interface>>
        +execute()
    }
    class MeleeAttackCommand
    class RangedAttackCommand
    class MoveMarchCommand
    class Mechanics {
        <<namespace>>
        tryMeleeAttack()
        tryRangedAttack()
        tryMoveTowardMarch()
    }

    class CommandResult
    class EventLogVisitor {
        <<visitor>>
    }

    Battle *-- BattleState
    Battle ..> UnitFactory : spawn
    Battle ..> EventLogVisitor : log

    BattleState *-- UnitRegistry
    BattleState *-- Map
    BattleState *-- BattleQueries
    BattleState ..> UnitBehaviorRegistry : processUnitActions

    UnitRegistry *-- ComponentStorage

    BattleQueries *-- CellOccupancy
    BattleQueries *-- CombatQueries
    BattleQueries *-- MarchMovement

    CellOccupancy ..> UnitRegistry
    CellOccupancy ..> Map
    CombatQueries ..> UnitRegistry
    MarchMovement ..> UnitRegistry
    MarchMovement ..> Map
    MarchMovement ..> CellOccupancy

    UnitFactory ..> BattleState
    UnitFactory ..> UnitDefinitionRegistry
    UnitDefinitionRegistry ..> UnitDefinition
    UnitDefinition ..> UnitRegistry

    UnitBehaviorRegistry ..> IUnitBehavior
    SwordsmanBehavior --|> IUnitBehavior
    HunterBehavior --|> IUnitBehavior

    IUnitBehavior ..> ICommand : selectCommand
    IUnitBehavior ..> BattleState

    MeleeAttackCommand --|> ICommand
    RangedAttackCommand --|> ICommand
    MoveMarchCommand --|> ICommand

    ICommand ..> BattleState
    ICommand ..> CommandResult
    MeleeAttackCommand ..> Mechanics
    RangedAttackCommand ..> Mechanics
    MoveMarchCommand ..> Mechanics

    Mechanics ..> BattleQueries
    Mechanics ..> UnitRegistry

    EventLogVisitor ..> CommandResult
```



**Кратко по слоям:**


| Слой        | Классы                                                              | Роль                             |
| ----------- | ------------------------------------------------------------------- | -------------------------------- |
| Оркестрация | `Battle`                                                            | сценарий, цикл тиков, связь с IO |
| Состояние   | `BattleState`, `Map`, `UnitRegistry`                                | данные боя                       |
| Запросы     | `BattleQueries` → `CellOccupancy`, `CombatQueries`, `MarchMovement` | spatial/combat без логики типов  |
| Юниты       | `UnitFactory`, `UnitDefinition`*                                    | создание и набор компонентов     |
| AI          | `IUnitBehavior`, `*Behavior`, `UnitBehaviorRegistry`                | выбор действия по типу           |
| Действия    | `ICommand`, `*Command`, `Mechanics`                                 | исполнение атаки/движения        |
| Лог         | `EventLogVisitor`                                                   | события → IO                     |


---

## Схема пошаговой работы

### Фаза 1 — загрузка сценария (тик 1)

```mermaid
sequenceDiagram
    participant main
    participant Battle
    participant Parser as CommandParser
    participant State as BattleState
    participant Factory as UnitFactory
    participant Log as EventLog

    main->>Battle: loadScenario(file)
    Battle->>State: reset()

    loop каждая строка сценария
        Parser->>Battle: CREATE_MAP / SPAWN_* / MARCH
        alt CREATE_MAP
            Battle->>State: createMap(W, H)
            Battle->>Log: MAP_CREATED
        else SPAWN_SWORDSMAN / SPAWN_HUNTER
            Battle->>Factory: spawn(state, command)
            Factory->>State: validateSpawn → registerUnit → apply components
            Battle->>Log: UNIT_SPAWNED
        else MARCH
            Battle->>State: startMarch(id, x, y)
            Battle->>Log: MARCH_STARTED
        end
    end

    main->>Battle: run()
```



### Фаза 2 — боевой цикл (тик 2, 3, …)

```mermaid
flowchart TD
    A[Battle::run] --> B{isFinished?<br/>alive ≤ 1 или<br/>!canAnyUnitAct}
    B -->|да| Z[конец]
    B -->|нет| C[performNextStep]

    C --> D[removeDeadUnits<br/>hp=0 → pendingRemoval<br/>UNIT_DIED]
    D --> E[processUnitActions<br/>порядок: _spawnOrder]

    E --> F{для каждого<br/>активного юнита}
    F --> G[UnitBehaviorRegistry::get(type)]
    G --> H[behavior.act(state, id)]
    H --> I[selectCommand → unique_ptr ICommand]
    I --> J{command<br/>!= null?}
    J -->|нет| F
    J -->|да| K[command.execute(state, id)]
    K --> L[Mechanics:<br/>melee / ranged / march]
    L --> M[CommandResult<br/>с событиями]
    M --> F

    F -->|все обработаны| N[logEvents → stdout]
    N --> O[tick++]
    O --> B
```



### Внутри одного хода юнита

```mermaid
flowchart LR
    subgraph Behavior
        SC[selectCommand]
    end

    subgraph Queries
        CO[CellOccupancy]
        CQ[CombatQueries]
        MM[MarchMovement]
    end

    subgraph Command
        EX[execute]
        ME[Mechanics]
    end

    SC --> CO
    SC --> CQ
    SC --> MM
    SC -->|Melee / Ranged / Move| EX
    EX --> ME
    ME --> CQ
    ME --> MM
    ME --> UR[UnitRegistry<br/>HP, Position, March]
```



**Приоритет действий (из behaviors):**


| Тип       | Порядок выбора                                                   |
| --------- | ---------------------------------------------------------------- |
| Swordsman | Ближняя атака или Марш (если некого атаковать)                   |
| Hunter    | Ближняя атака или Дальняя атака или Марш (если некого атаковать) |


---

Так как в задании прямо сказано: *«Не беспокойтесь о производительности»*, то не все узкие места были оптимизированы. Ниже список того, что еще можно было бы оптимизировать:

### 1. Двойной вызов AI на каждом тике (самое заметное)

**Где:** `Battle::run()` → `isFinished()` → `canAnyUnitAct()` → `canAct()` → `selectCommand()`, затем `performNextStep()` → `act()` → `selectCommand()` снова.

**Вариант улучшения:** один проход «выбрать → выполнить» в `processUnitActions`, флаг продолжения после тика вместо `canAnyUnitAct()` перед тиком.

---

### 2. Spatial-запросы O(N) по всем юнитам

**Где:** `CellOccupancy`, `CombatQueries`, цепочки вызовов из behaviors.

**Вариант улучшения:** grid-индекс `map[x][y] → EntityId` или кэш запросов на один тик.

---

### 3. Мёртвые юниты остаются в `_spawnOrder`

**Где:** `UnitRegistry::_spawnOrder`, `setPendingRemoval()`.

**Вариант улучшения:** `UnitRegistry::removeUnit()` в начале следующего тикас (*«исчезает на следующий ход»)*.

---

### 4. Лишние аллокации `std::vector`

**Где:** API queries возвращает owning-контейнер, behaviors проверяют только `.empty()`.

**Вариант улучшения:** методы `hasAdjacentEnemies()` / `hasEnemiesInRange()` без vector; `selectCommand()` возвращает лёгкий `enum`/optional вместо `unique_ptr` при проверке; thread-local / reusable buffer для списка целей при атаке.

---

