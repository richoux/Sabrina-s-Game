#include <iostream>
#include "utils.hpp"

std::pair<int,int> index_to_coord( int index, int width ) 
{
	return std::pair<int,int>( index / width, index % width );
}

int coord_to_index( int first, int second, int width )
{
	return first * width + second;
}

void display_solution( const std::vector< std::pair<int,int> >& solution, int width )
{
	std::cout << "Solution: ";
	for( auto &pair_of_index: solution )
	{
		auto c1 = index_to_coord( pair_of_index.first, width );
		auto c2 = index_to_coord( pair_of_index.second, width );

		if( c1.first > c2.first || c1.second > c2.second )
			std::swap( c1, c2 );
		
		std::cout << "[(" << c1.first << "," << c1.second << ") (" << c2.first << "," << c2.second << ")] ; ";
	}
	std::cout << "\n";
}

void display_solution_short( const std::vector< std::pair<int,int> >& solution )
{
	for( auto &pair_of_index: solution )
		std::cout << pair_of_index.first << " " << pair_of_index.second << "\n";
	std::cout << "\n";
}
