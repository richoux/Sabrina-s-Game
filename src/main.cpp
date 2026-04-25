#include <iostream>
#include "build.hpp"
#include "utils.hpp"

///////////////////////

int main( int argc, char **argv )
{
	int w,h;

	if( argc != 3 )
	{
		std::cout << "Usage: " << argv[0] << " width height\n";
		return EXIT_FAILURE;
	}
	else
	{
		w = std::stoi( argv[1] );
		h = std::stoi( argv[2] );
	}

	sabrinasgame::Solution solution( w, h );
	auto raw_sol = solution.build();

	std::cout << "Solution: ";
	for( auto &pair_of_index: raw_sol )
	{
		auto c1 = index_to_coord(pair_of_index.first, w);
		auto c2 = index_to_coord(pair_of_index.second, w);

		if( c1.first > c2.first || c1.second > c2.second)
			std::swap( c1, c2 );
		
		std::cout << "[(" << c1.first << "," << c1.second << ") (" << c2.first << "," << c2.second << ")] ; ";
	}
	std::cout << "\n";
	
	solution.print();
	
	return EXIT_SUCCESS;
}
