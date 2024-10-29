#pragma once

#include <Common.h>

namespace sw {

class Unit;

// Интерфейс валидатора данных юнита.
struct IUnitValidator
{
	virtual ~IUnitValidator() = default;

	// Посетить юнит для проверки. Выбросит исключение если проверка провалилась.
	virtual void Visit( const Unit& unit ) = 0;
};

} // namespace sw