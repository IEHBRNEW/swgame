#pragma once

#include <Controller/Validators/UnitValidator.h>
#include <Shared/Types.hpp>
#include <Models/Units/Unit.h>

namespace sw {

// Имеет состояние. Нельзя сохранять между проверками.
class UnitDoubleIdValidator final : public IUnitValidator
{
public:
    UnitDoubleIdValidator() {}
    virtual ~UnitDoubleIdValidator() = default;

    // IUnitValidator
    void Visit( const Unit& unit ) override
    {
        if( unitIds.contains( unit.GetId() ) )
            throw std::runtime_error( std::format( "Error: Multiple units have the same id: {}", unit.GetId() ) );
        unitIds.insert( unit.GetId() );
    }

private:
    std::unordered_set<uint32_t> unitIds;
};

} // namespace sw