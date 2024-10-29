#pragma once

#include <Models/Units/Unit.h>

namespace sw {

class Archer final : public Unit
{
public:
	Archer( uint32_t id, uint32_t hp, uint32_t strength, uint32_t agility, uint32_t range, const Point& position, IBattleObserver* observer );
	virtual ~Archer() = default;

	// Unit
	std::string GetName() const override { return Name; }

protected:
	// Unit
	bool TryAttack( IBattleScene* scene ) const override;

private:
	constexpr static const char* Name = "Archer";
};

} // namespace sw