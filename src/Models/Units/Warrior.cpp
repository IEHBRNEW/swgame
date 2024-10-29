#include <Common.h>

#include <Models/Units/Warrior.h>
#include <Models/Units/Attacks/MeleeAttack.hpp>

namespace sw {

Warrior::Warrior( uint32_t id, uint32_t hp, uint32_t strength, const Point& position, IBattleObserver* observer ) :
	Unit( id, hp, position, observer )
{
	AddAttackStrategy( std::make_unique<MeleeAttack>( strength ) );
}

bool Warrior::TryAttack( IBattleScene* scene ) const
{
	auto* meleeStrategy = GetAttackStrategy( AttackType::MeleeAttack );
	assert( meleeStrategy != nullptr );
	return AttackWithStrategy( meleeStrategy, scene );
}

} // namespace sw