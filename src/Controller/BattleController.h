#pragma once

#include <Controller/Interfaces/BattleScene.h>
#include <Controller/Interfaces/BattleObserver.h>
#include <Models/Units/Unit.h>
#include <Shared/Types.hpp>
#include <IO/System/EventLog.hpp>

namespace sw {

// Класс проводящий и контролирующий симуляцию.
class BattleController final : public IBattleObserver, public IBattleScene
{
public:
	explicit BattleController( std::vector<Command>&& commands );
	virtual ~BattleController() = default;

	// IBattleObserver
	void OnUnitMoved( const Unit& unit ) override;
	void OnUnitAttacked( const Unit& attacker, const Unit& target, uint32_t damage ) override;

	// IBattleScene
	const std::vector<std::unique_ptr<Unit>>& GetUnits() override;
	const Size& GetMapSize() const override;

	// Запустить симуляцию. Повторный запуск невозможен.
	void Run();

private:
	EventLog m_eventLog; // Лог событий.
	bool m_hasRun; // Был ли запуск симуляции.
	uint64_t m_currentStep; // Текущий счетчик шага симуляции.

	Size m_map; // Размер карты.
	std::vector<Command> m_commands; // Список инициирующих команд.
	std::vector<std::unique_ptr<Unit>> m_units; // Юниты.

	void checkAlreadyRun();
	bool isMoreThenOneAlive() const;
	void runInitCommandsAndValidate();
	void runBattleLoop();

	void runCommand( const io::CreateMap& command );
	void runCommand( const io::SpawnWarrior& command );
	void runCommand( const io::SpawnArcher& command );
	void runCommand( const io::March& command );
};

} // namespace sw