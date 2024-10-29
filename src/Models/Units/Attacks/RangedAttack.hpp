#pragma once

#pragma once

#include <Models/Units/Attacks/AttackStrategy.h>

namespace sw {

class RangedAttack final : public AttackStrategy
{
public:
	RangedAttack( uint32_t damage, uint32_t range ) : m_damage( damage ), m_range( range ) {
		if( range < MinRange )
			throw std::runtime_error( "Error: Invalid range" );
	}
	virtual ~RangedAttack() = default;

	// AttackStrategy
	AttackType GetAttackType() const override {	return AttackType::RangedAttack; }
	uint32_t GetDamage() const override { return m_damage; }
	std::optional<Unit*> GetAttackTarget( const Unit& attacker, IBattleScene* scene ) const override
		{ return GetTargetInRange( MinRange, m_range, attacker, scene ); }
	void Attack( Unit& target ) const override 
		{ target.AcceptDamage( m_damage ); }

private:
	constexpr static uint32_t MinRange = 2;

	const uint32_t m_damage;
	const uint32_t m_range;
};

} // namespace sw