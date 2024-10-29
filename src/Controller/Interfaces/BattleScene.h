#pragma once

#include <Common.h>

namespace sw {

class Unit;
struct Size;

// Интерфейс сцены симуляции. Отвечает за доступ к данным для совершения хода.
struct IBattleScene
{
	virtual ~IBattleScene() = default;

	// Получить доступ к юнитам симуляции.
	virtual const std::vector<std::unique_ptr<Unit>>& GetUnits() = 0;
	// Получить размер карты.
	virtual const Size& GetMapSize() const = 0;
};

} // namespace sw