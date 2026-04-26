#include <iostream>
#include "build.hpp"
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
	
	int number_solutions = compute_combinatorics(w,h);
	std::cout << "Number of valid solutions for a (" << w << "x" << h << ") rectangle: " << number_solutions << "\n";
		
	sabrinasgame::Solution solution( w, h );
	std::vector< std::pair<int,int> > result;
	
	if( xp == 1 )
	{
		result = solution.build();
		display_solution( result, w );
		solution.print();
	}
	else
	{
		for( int run = 1 ; run <= xp ; ++run )
		{
			std::cout << "Run #" << run << "\n";
			result = solution.build();
			display_solution_short( result );
		}
	}
	
	return EXIT_SUCCESS;
}
