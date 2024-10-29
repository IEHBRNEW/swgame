#include <Common.h>
#include <Controller/BattleController.h>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/PrintDebug.hpp>
#include <Shared/Types.hpp>

using namespace sw;
using namespace sw::io;

namespace {

template<typename T>
auto addCommand( std::vector<Command>& commands )
{
	return [&commands]( T&& command )
		{
			printDebug( std::cout, command );
			commands.emplace_back( std::forward<T>( command ) );
		};
}

std::vector<Command> parse( const std::string& fileName )
{
	std::ifstream file( fileName );
	if( !file )
	{
		throw std::runtime_error( "Error: File not found - " + fileName );
	}

	std::vector<Command> commands;
	CommandParser parser;

	parser.add<CreateMap>( addCommand<CreateMap>( commands ) )
		.add<SpawnWarrior>( addCommand<SpawnWarrior>( commands ) )
		.add<SpawnArcher>( addCommand<SpawnArcher>( commands ) )
		.add<March>( addCommand<March>( commands ) );

	parser.parse( file );
	return commands;
}

} // namespace

int main( int argc, char** argv )
{
	if( argc != 2 )
		throw std::runtime_error( "Error: No file specified in command line argument" );

	BattleController controller( parse( argv[1] ) );
	controller.Run();

	return 0;
}
