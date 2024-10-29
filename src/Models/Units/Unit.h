#pragma once

#include <Shared/Types.hpp>
#include <Models/Units/Attacks/AttackStrategy.h>
#include <Controller/Interfaces/BattleObserver.h>
#include <Controller/Interfaces/BattleScene.h>

namespace sw {

struct IUnitValidator;

template<typename T>
concept DerivedFromAttackStrategy = std::is_base_of<AttackStrategy, T>::value;

// Базовый класс юнита в симуляции.
class Unit
{
public:
	Unit( uint32_t id, uint32_t hp, const Point& position, IBattleObserver* observer );
	virtual ~Unit() = default;

	// Получить имя класса юнита.
	virtual std::string GetName() const = 0;

	uint32_t GetId() const { return m_id; }
	uint32_t GetHp() const { return m_hp; }
	bool IsAlive() const { return m_hp > 0; }
	void AcceptDamage( uint32_t damage ) { m_hp = m_hp > damage ? m_hp - damage : 0; }

	// Получить текущую позицию.
	const Point& GetPosition() const { return m_pos; }
	// Получить позицию, которую юнит стремится достичь во время симуляции. 
	// Сгенерирует исключение, если позиция не была задана.
	const Point& GetTargetPosition() const { return m_targetPos.value(); }
	// Проверить есть ли конечная позиция для передвижения.
	bool HasTargetPosition() const { return m_targetPos.has_value(); }
	// Установить целевую позицию.
	void SetTargetPosition( const Point& target ) {	m_targetPos = target; }

	// Сделать ход. Вернет true, если что-то произошло (атака или движение).
	bool MakeMove( IBattleScene* scene );

	void Accept( IUnitValidator* validator ) const;

protected:
	// Атаковать в свой ход. Вернет true, если атака была совершена.
	virtual bool TryAttack( IBattleScene* scene ) const = 0;
	// Передвинуться в свой ход. Вернет true, если передвижение произошло.
	virtual bool TryShift();

	// Добавить стратегию атаки.
	void AddAttackStrategy( std::unique_ptr<AttackStrategy> strategy );
	// Получить стратегию атаки по типу. 
	// Сгенерирует исключение, если стратегии с таким типом не задано.
	AttackStrategy* GetAttackStrategy( AttackType type ) const;

	// Совершить атаку со стратегией. Вернет true, если атака была совершена.
	template<DerivedFromAttackStrategy T>
	bool AttackWithStrategy( T* strategy, IBattleScene* scene ) const
	{
		auto targetUnit = strategy->GetAttackTarget( *this, scene );
		if( targetUnit )
		{
			strategy->Attack( *targetUnit.value() );
			m_observer->OnUnitAttacked( *this, *targetUnit.value(), strategy->GetDamage() );
			return true;
		}
		return false;
	}

private:
	uint32_t m_id;
	uint32_t m_hp;
	Point m_pos;
	std::optional<Point> m_targetPos; // Позиция, которую юнит стремится достичь во время симуляции.
	std::unordered_map<AttackType, std::unique_ptr<AttackStrategy>> m_attackStrategies;

	IBattleObserver* m_observer;
};

} // namespace sw