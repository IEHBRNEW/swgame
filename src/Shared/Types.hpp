#pragma once

#include <Common.h>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>

namespace sw {

using Command = std::variant<io::CreateMap, io::SpawnWarrior, io::SpawnArcher, io::March>;

struct Point
{
	uint32_t x{};
	uint32_t y{};

	bool operator==( const Point& other ) const = default;

	std::pair<uint32_t, uint32_t> CalcDistance( const Point& other ) const
	{
		uint32_t dx = ( other.x > x ) ? ( other.x - x ) : ( x - other.x );
		uint32_t dy = ( other.y > y ) ? ( other.y - y ) : ( y - other.y );
		return { dx, dy };
	}

	struct Hash
	{
		std::size_t operator()( const Point& p ) const noexcept
		{
			return std::hash<uint32_t>{}( p.x ) ^ ( std::hash<uint32_t>{}( p.y ) << 1 );
		}
	};
};

struct Size
{
	uint32_t width{};
	uint32_t height{};

	bool IsEmpty() const { return width == 0 || height == 0; }
};

} // namespace sw