#include <Common.h>

#include <Models/Units/Unit.h>

#include <Controller/Validators/UnitValidator.h>
#include <Models/Units/Attacks/AttackStrategy.h>

namespace sw {

Unit::Unit( uint32_t id, uint32_t hp, const Point& position, IBattleObserver* observer ) :
	m_id( id ),
	m_hp( hp ),
	m_pos( position ),
	m_observer( observer )
{
	assert( m_observer != nullptr );
}

void Unit::AddAttackStrategy( std::unique_ptr<AttackStrategy> strategy )
{
	auto type = strategy->GetAttackType();
	m_attackStrategies[type] = std::move( strategy );
}

AttackStrategy* Unit::GetAttackStrategy( AttackType type ) const
{
	return m_attackStrategies.at( type ).get();
}

bool Unit::MakeMove( IBattleScene* scene )
{
	if( !IsAlive() )
		return false;

	if( TryAttack( scene ) )
		return true;

	return TryShift();
}

bool Unit::TryShift()
{
	if( !m_targetPos || m_targetPos == m_pos )
		return false;

	if( m_targetPos.value().x != m_pos.x )
		m_targetPos.value().x > m_pos.x ? m_pos.x++ : m_pos.x--;
	if( m_targetPos.value().y != m_pos.y )
		m_targetPos.value().y > m_pos.y ? m_pos.y++ : m_pos.y--;

	m_observer->OnUnitMoved( *this );

	return true;
}

void Unit::Accept( IUnitValidator* validator ) const
{
	return validator->Visit( *this );
}

} // namespace sw
