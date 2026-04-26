#include <iostream>
#include <cmath>
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

int compute_combinatorics( int width, int height )
{
	// see https://en.wikipedia.org/wiki/Domino_tiling#Counting_tilings_of_regions
	double tmp1, tmp2;
	double total = 1.;
	for( int w = 1 ; w <= width/2 ; ++w )
	{
		tmp1 = std::cos(w*std::numbers::pi / (width+1));
		for( int h = 1 ; h <= height/2 ; ++h )
		{
			tmp2 = std::cos(h*std::numbers::pi / (height+1));
			total *= 4*tmp1*tmp1 + 4*tmp2*tmp2;
		}
	}

	return static_cast<int>( total );
}
