#include <iostream>
#include <cmath>
#include <set>
#include "utils.hpp"

auto compare = [](std::pair<int,int> a, std::pair<int,int> b){ return std::min(a.first, a.second) < std::min(b.first, b.second);};

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
	std::set<std::pair<int,int>, decltype(compare)> sorted_solution;
	for( auto &pair_of_index: solution )
		sorted_solution.insert( pair_of_index );

	std::cout << "Solution: ";
	for( auto &s: sorted_solution )
	{
		auto c1 = index_to_coord( s.first, width );
		auto c2 = index_to_coord( s.second, width );

		// if( c1.first > c2.first || c1.second > c2.second )
		// 	std::swap( c1, c2 );
		
		std::cout << "[(" << c1.first << "," << c1.second << ") (" << c2.first << "," << c2.second << ")] ; ";
	}
	std::cout << "\n";
}

void display_solution_short( const std::vector< std::pair<int,int> >& solution )
{
	std::set<std::pair<int,int>, decltype(compare)> sorted_solution;
	for( auto &pair_of_index: solution )
		sorted_solution.insert( pair_of_index );
	
	for( auto &s: sorted_solution )
		std::cout << s.first << " " << s.second << "\n";
	std::cout << "\n";
}

std::stringstream get_normalized_solution( const std::vector< std::pair<int,int> >& solution, int width )
{
	std::set<std::pair<int,int>, decltype(compare)> sorted_solution;
	for( auto &pair_of_index: solution )
		sorted_solution.insert( pair_of_index );

	int index_plus_one = sorted_solution.begin()->first+1;
	std::stringstream normalized_solution;
	bool first_loop = true;
	
	for( auto iter = sorted_solution.begin() ; iter != sorted_solution.end() ; ++iter )
	{
		index_plus_one = iter->first+1;
		if( first_loop )
			first_loop = false;
		else
			normalized_solution << "_";
		
		if( width == 1 )
			normalized_solution << -index_plus_one; // vertical
		else
			if( iter->second == iter->first + 1 )
				normalized_solution << index_plus_one; // horizontal
			else
				normalized_solution << -index_plus_one; // vertical
	}
	normalized_solution << "\n";
	return normalized_solution;
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
