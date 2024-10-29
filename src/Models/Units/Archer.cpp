#include <Common.h>

#include <Models/Units/Archer.h>
#include <Models/Units/Attacks/MeleeAttack.hpp>
#include <Models/Units/Attacks/RangedAttack.hpp>

namespace sw {

Archer::Archer( uint32_t id, uint32_t hp, uint32_t strength, uint32_t agility, uint32_t range, const Point& position, IBattleObserver* observer ) :
	Unit( id, hp, position, observer )
{
	AddAttackStrategy( std::make_unique<MeleeAttack>( strength ) );
	AddAttackStrategy( std::make_unique<RangedAttack>( agility, range ) );
}

bool Archer::TryAttack( IBattleScene* scene ) const
{
	auto* meleeStrategy = GetAttackStrategy( AttackType::MeleeAttack );
	if( AttackWithStrategy( meleeStrategy, scene ) )
		return true;

	auto* rangedStrategy = GetAttackStrategy( AttackType::RangedAttack );
	if( AttackWithStrategy( rangedStrategy, scene ) )
		return true;

	return false;
}

} // namespace sw