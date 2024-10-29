#pragma once

#include <Controller/Validators/UnitValidator.h>
#include <Shared/Types.hpp>
#include <Models/Units/Unit.h>

namespace sw {

class UnitPositionValidator final : public IUnitValidator
{
public:
    explicit UnitPositionValidator( const Size& mapSize ) : m_mapSize( mapSize ) {}
    virtual ~UnitPositionValidator() = default;

    // IUnitValidator
    void Visit( const Unit& unit ) override
    {
        const Point& pos = unit.GetPosition();
        if( pos.x >= m_mapSize.width || pos.y >= m_mapSize.height )
            throw std::runtime_error( std::format( "Error: Unit {} position out of bounds", unit.GetId() ) );
    }

private:
    Size m_mapSize;
};

} // namespace sw