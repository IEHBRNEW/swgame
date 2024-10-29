#include <Common.h>

#include <Controller/BattleController.h>

#include <Controller/Validators/UnitDoublePositionValidator.hpp>
#include <Controller/Validators/UnitDoubleIdValidator.hpp>
#include <Controller/Validators/UnitPositionValidator.hpp>
#include <Controller/Validators/UnitTargetPositionValidator.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/System/EventLog.hpp>
#include <Models/Units/Attacks/AttackStrategy.h>
#include <Models/Units/Unit.h>
#include <Models/Units/Warrior.h>
#include <Models/Units/Archer.h>
#include <Shared/Types.hpp>

namespace sw {

BattleController::BattleController( std::vector<Command>&& commands ) :
	m_hasRun( false ),
	m_currentStep( 1 ),
	m_commands( std::move( commands ) )
{
}

void BattleController::OnUnitMoved( const Unit& unit )
{
	m_eventLog.log( m_currentStep, io::March{ unit.GetId(), unit.GetPosition().x, unit.GetPosition().y } );
	if( unit.GetPosition() == unit.GetTargetPosition() )
		m_eventLog.log( m_currentStep, io::MarchEnded{ unit.GetId(), unit.GetPosition().x, unit.GetPosition().y } );
}

void BattleController::OnUnitAttacked( const Unit& attacker, const Unit& target, uint32_t damage )
{
	m_eventLog.log( m_currentStep, io::UnitAttacked{ attacker.GetId(), target.GetId(), damage, target.GetHp() } );
	if( !target.IsAlive() )
		m_eventLog.log( m_currentStep, io::UnitDied{ target.GetId() } );
}

const std::vector<std::unique_ptr<Unit>>& BattleController::GetUnits()
{
	return m_units;
}

const Size& BattleController::GetMapSize() const
{
	return m_map;
}

void BattleController::Run()
{
	checkAlreadyRun();

	runInitCommandsAndValidate();
	runBattleLoop();
}

void BattleController::checkAlreadyRun()
{
	if( m_hasRun )
		throw std::runtime_error( "Error: BattleController run() called more than once" );
	m_hasRun = true;
}

void BattleController::runInitCommandsAndValidate()
{
	// Запустить команды скрипта.
	for( const auto& command : m_commands )
	{
		std::visit( [this]( const auto& command )
			{
				runCommand( command );
			}, command );
	}

	// Провалидировать результат.
	if(m_map.IsEmpty())
		throw std::runtime_error( "Error: Map size can not be empty" );

	std::vector<std::unique_ptr<IUnitValidator>> unitValidators;
	unitValidators.emplace_back( std::make_unique<UnitPositionValidator>( m_map ) );
	unitValidators.emplace_back( std::make_unique<UnitTargetPositionValidator>( m_map ) );
	unitValidators.emplace_back( std::make_unique<UnitDoubleIdValidator>() );
	unitValidators.emplace_back( std::make_unique<UnitDoublePositionValidator>() );

	for( const auto& unit : m_units )
	{
		for( const auto& validator : unitValidators )
			unit->Accept( validator.get() );
	}

	m_currentStep++;
}

void BattleController::runBattleLoop()
{
	for( ; m_currentStep < std::numeric_limits<uint64_t>::max(); m_currentStep++ )
	{
		bool moved = false;
		for( const auto& unitPtr : m_units )
		{
			if( unitPtr->MakeMove( this ) )
				moved = true;
		}
				
		if( !moved || !isMoreThenOneAlive() )
			break;
	}
}

bool BattleController::isMoreThenOneAlive() const
{
	auto alive = std::count_if( m_units.begin(), m_units.end(), []( const auto& unitPtr )
		{
			return unitPtr->IsAlive();
		} );
	return alive > 1;
}

void BattleController::runCommand( const io::CreateMap& command )
{
	m_map = { command.width, command.height };
	m_eventLog.log( m_currentStep, io::MapCreated{ command.width, command.height } );
}

void BattleController::runCommand( const io::SpawnWarrior& command )
{
	m_units.push_back( std::make_unique<Warrior>( command.unitId, command.hp, command.strength,
		Point{ command.x, command.y }, this ) );
	m_eventLog.log( m_currentStep, io::UnitSpawned{ command.unitId, m_units.back()->GetName(), command.x, command.y } );
}

void BattleController::runCommand( const io::SpawnArcher& command )
{
	m_units.push_back( std::make_unique<Archer>( command.unitId, command.hp, command.strength,
		command.agility, command.range, Point{ command.x, command.y }, this ) );
	m_eventLog.log( m_currentStep, io::UnitSpawned{ command.unitId, m_units.back()->GetName(), command.x, command.y } );
}

void BattleController::runCommand( const io::March& command )
{
	auto it = std::find_if( m_units.begin(), m_units.end(),
		[unitId = command.unitId]( const std::unique_ptr<Unit>& unit )
		{
			return unit->GetId() == unitId;
		} );
	if( it == m_units.end() )
		throw std::runtime_error( "Error: Can't find unit to start march" );
	( *it )->SetTargetPosition( Point{ command.targetX, command.targetY } );
	const Point& pos = ( *it )->GetPosition();
	m_eventLog.log( m_currentStep, io::MarchStarted{ command.unitId, pos.x, pos.y, command.targetX, command.targetY } );
}

} // namespace sw