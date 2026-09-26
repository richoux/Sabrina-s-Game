#include <iostream>
#include <cassert>

#include "ford_fulkerson.hpp"
#include "graph.hpp"
#include "utils.hpp"

///////////////////////

int main( int argc, char **argv )
{
	int w,h,xp;

	if( argc < 3 || argc > 4 )
	{
		std::cout << "Usage: " << argv[0] << " width height [#xp_runs]\n";
		return EXIT_FAILURE;
	}
	else
	{
		w = std::stoi( argv[1] );
		h = std::stoi( argv[2] );
		if( argc == 4 )
			xp = std::stoi( argv[3] );
		else
			xp = 1;
	}

	if( w % 2 == 1 && h % 2 == 1 )
	{
		std::cout << "Width and height cannot be both odd.\n";
		return EXIT_FAILURE;
	}

	bool printable_solution = ( w * h < 100 );
	
	long long number_solutions = compute_combinatorics(w,h);
	std::cout << "Number of valid solutions for a (" << w << "x" << h << ") rectangle: " << number_solutions << "\n";
	
		
	if( xp == 1 )
	{
		sabrinasgame::FordFulkerson ff( w, h );
		auto flow = ff.solve();
		display_solution( flow.edges_from_white_nodes(), w );
		ff.print();
		assert( check_solution( flow.edges_from_white_nodes(), w, h ) );			
	}
	else
	{
		for( int run = 1 ; run <= xp ; ++run )
		{
			std::cout << "Run #" << run << " | ";
			sabrinasgame::FordFulkerson ff( w, h );
			auto flow = ff.solve();

			std::cout << "1 | ";
			if( printable_solution )
			{
				auto norm_sol = get_normalized_solution( flow.edges_from_white_nodes(), w );
				std::cout << norm_sol.str();
			}
			else
				std::cout << "\n";

			assert( check_solution( flow.edges_from_white_nodes(), w, h ) );			
		}
	}

	return EXIT_SUCCESS;
}
