#pragma once

#include <Controller/Validators/UnitValidator.h>
#include <Shared/Types.hpp>
#include <Models/Units/Unit.h>

namespace sw {

// Имеет состояние. Нельзя сохранять между проверками.
class UnitDoublePositionValidator final : public IUnitValidator
{
public:
    UnitDoublePositionValidator() {}
    virtual ~UnitDoublePositionValidator() = default;

    // IUnitValidator
    void Visit( const Unit& unit ) override
    {
        if( !unit.IsAlive() )
            return;

        if( unitPositions.contains( unit.GetPosition() ) )
            throw std::runtime_error( std::format( "Error: Multiple units have the same position: {} {}", unit.GetPosition().x, unit.GetPosition().y ) );
        unitPositions.insert( unit.GetPosition() );
    }

private:
    std::unordered_set<Point, Point::Hash> unitPositions;
};

} // namespace sw