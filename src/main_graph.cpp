#include <iostream>
#include <cassert>

#if defined BFS
#include "edmonds_karp.hpp"
#else
#include "ford_fulkerson.hpp"
#endif

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

	bool printable_solution = ( w * h < 1000 );
	
	long long number_solutions = compute_combinatorics(w,h);
	std::cout << "Number of valid solutions for a (" << w << "x" << h << ") rectangle: " << number_solutions << "\n";
	
		
	if( xp == 1 )
	{
#if defined BFS
		sabrinasgame::EdmondsKarp ff( w, h );
#else
		sabrinasgame::FordFulkerson ff( w, h );
#endif
		auto flow = ff.solve();
		auto edges = flow.edges_from_white_nodes();
		display_solution( edges, w );
		ff.print();
		assert( check_solution( edges, w, h ) );			
	}
	else
	{
#if defined BFS
		std::cout << "Run | Status | Normalized solution (if not too long) | Number of solver iterations | Runtime in us | Memory usage in kb\n";
#endif
		for( int run = 1 ; run <= xp ; ++run )
		{
			if( run % 10000 == 0 )
				std::cerr << run << "\n";
			
			std::cout << "Run #" << run << " | ";
#if defined BFS
			sabrinasgame::EdmondsKarp ff( w, h );
#else
			sabrinasgame::FordFulkerson ff( w, h );
#endif
			auto flow = ff.solve();
			auto edges = flow.edges_from_white_nodes();

			std::cout << "1 | ";
			if( printable_solution )
			{
				auto norm_sol = get_normalized_solution( edges, w );
				std::cout << norm_sol.str();
			}

#if defined BFS
			// Runtimes in us, memory usage in kb
			std::cout << " | " << ff.get_number_iterations()
			          << " | " << ff.get_runtime()
			          << " | " << ff.get_memory_usage() << "\n";
#else
			std::cout << "\n";
#endif
			
			assert( check_solution( edges, w, h ) );			
		}
	}

	return EXIT_SUCCESS;
}
