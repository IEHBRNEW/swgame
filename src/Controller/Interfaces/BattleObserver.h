#pragma once

#include <Common.h>

namespace sw {

class Unit;

// Интерфейс обработчика совершенных действий симуляции.
struct IBattleObserver
{
	virtual ~IBattleObserver() = default;

	// Юнит передвинулся.
	virtual void OnUnitMoved( const Unit& unit ) = 0;
	// Юнит атаковал другого юнита.
	virtual void OnUnitAttacked( const Unit& attacker, const Unit& target, uint32_t damage ) = 0;
};

} // namespace sw