#pragma once

#include <Models/Units/Unit.h>

namespace sw {

class Warrior final : public Unit
{
public:
	Warrior( uint32_t id, uint32_t hp, uint32_t strength, const Point& position, IBattleObserver* observer );
	virtual ~Warrior() = default;

	// Unit
	std::string GetName() const override { return Name; }

protected:
	// Unit
	bool TryAttack( IBattleScene* scene ) const override;

private:
	constexpr static const char* Name = "Warrior";
};

} // namespace sw