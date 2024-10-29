#pragma once

#include <Controller/Validators/UnitValidator.h>
#include <Shared/Types.hpp>
#include <Models/Units/Unit.h>

namespace sw {

class UnitTargetPositionValidator final : public IUnitValidator
{
public:
    explicit UnitTargetPositionValidator( const Size& mapSize ) : m_mapSize( mapSize ) {}
    virtual ~UnitTargetPositionValidator() = default;

    // IUnitValidator
    void Visit( const Unit& unit ) override
    {
        if( !unit.HasTargetPosition() )
            return;

        const Point& pos = unit.GetTargetPosition();
        if( pos.x >= m_mapSize.width || pos.y >= m_mapSize.height )
            throw std::runtime_error( std::format( "Error: Unit {} target position out of bounds", unit.GetId() ) );
    }

private:
    Size m_mapSize;
};

} // namespace sw