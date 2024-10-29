#include <Common.h>
#include <Models/Units/Attacks/AttackStrategy.h>
#include <Models/Units/Unit.h>

namespace sw {

std::optional<Unit*> AttackStrategy::GetTargetInRange( uint32_t low, uint32_t high, const Unit& attacker, IBattleScene* scene ) const
{
	assert( low <= high );

	std::optional<Unit*> result;
	for( const auto& punit : scene->GetUnits() )
	{
		if( punit->GetId() == attacker.GetId() || !punit->IsAlive() )
			continue;

		auto [dx, dy] = attacker.GetPosition().CalcDistance( punit->GetPosition() );
		auto max = std::max( dx, dy );
		if( max < low || max > high )
			continue;

		if( !result )
		{
			result = punit.get();
			continue;
		}

		// В задаче не сказано как выбирается противник из равных целей.
		// Для детерминированности будем искать самого слабого.
		if( result.value()->GetHp() > punit->GetHp() )
			result = punit.get();
	}
	return result;
}

} // namespace sw