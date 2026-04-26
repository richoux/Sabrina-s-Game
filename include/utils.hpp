#pragma once

#include <vector>
#include <utility> //std::pair

std::pair<int,int> index_to_coord( int index, int );
int coord_to_index( int, int, int );
void display_solution( const std::vector< std::pair<int,int> >& solution, int width );
void display_solution_short( const std::vector< std::pair<int,int> >& solution );
int compute_combinatorics( int w, int h );
