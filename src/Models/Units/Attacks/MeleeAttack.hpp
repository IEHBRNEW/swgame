#pragma once

#include <Models/Units/Attacks/AttackStrategy.h>

namespace sw {

class MeleeAttack final : public AttackStrategy
{
public:
	explicit MeleeAttack( uint32_t damage ) : m_damage( damage ) {}
	virtual ~MeleeAttack() = default;

	// AttackStrategy
	AttackType GetAttackType() const override { return AttackType::MeleeAttack; }
	uint32_t GetDamage() const override { return m_damage; }
	std::optional<Unit*> GetAttackTarget( const Unit& attacker, IBattleScene* scene ) const override
		{ return GetTargetInRange( 1, 1, attacker, scene ); }
	void Attack( Unit& target ) const override { target.AcceptDamage( m_damage ); }

private:
	const uint32_t m_damage;
};

} // namespace sw 