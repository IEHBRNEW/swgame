#pragma once

#include <Models/Units/Attacks/AttackType.h>

namespace sw {

class Unit;

struct IBattleScene;

// Базовый класс атакующей стратегии. Каждый юнит может иметь одну или несколько атакующих стратегий и использовать их при атаке.
class AttackStrategy
{
public:
	virtual ~AttackStrategy() = default;

	// Тип атаки.
	virtual AttackType GetAttackType() const = 0;
	// Возможный урон при атаке.
	virtual uint32_t GetDamage() const = 0;

	// Получить оптимальную для атаки цель, которую можно атаковать этой стратегией.
	virtual std::optional<Unit*> GetAttackTarget( const Unit& attacker, IBattleScene* scene ) const = 0;
	// Произвести атаку.
	virtual void Attack( Unit& target ) const = 0;

protected:
	// Получить оптимальную для атаки цель, находящуюся на расстоянии от low (включительно) до high (включительно).
	// Например для атаки в рамках 1 ближайщей клетки low == 1, high == 1.
	// Оптимальной считается цель, у которой меньше всего hp из доступных целей.
	std::optional<Unit*> GetTargetInRange( uint32_t low, uint32_t high, const Unit& attacker, IBattleScene* scene ) const;
};

} // namespace sw