#include "utils.hpp"

std::pair<int,int> index_to_coord( int index, int width ) 
{
	return std::pair<int,int>( index / width, index % width );
}

int coord_to_index( int first, int second, int width )
{
	return first * width + second;
}

