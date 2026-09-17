#include <iostream>

#include "graph.hpp"
#include "utils.hpp"

using sabrinasgame::Graph;

Graph::Graph( int width, int height, bool empty )
	: _width(width),
	  _height(height),
	  _nb_cells(width * height + 2), // +2 because we add a source and a sink
	  _source(_nb_cells-2),
	  _sink(_nb_cells-1),
	  _edges(std::vector<std::deque<int>>(_nb_cells)),
	  _white_cells(std::vector<int>()),
	  _black_cells(std::vector<int>())
{
	for( int c = 0 ; c < _nb_cells - 2 ; ++c )
	{
		int row = index_to_coord( c, _width ).first;
		
		if( _width % 2 == 1 )
		{
			if( c % 2 == 0 )
				_white_cells.push_back( c );
			else
				_black_cells.push_back( c );
		}
		else
		{
			if( row % 2 == c % 2 )
				_white_cells.push_back( c );
			else
				_black_cells.push_back( c );
		}
	}

	if( !empty )
	{
		for( auto& white : _white_cells )
			add_edge( _source, white );

		for( auto& black : _black_cells )
			add_edge( black, _sink );

		for( int c = 0 ; c < _nb_cells - 2 ; ++c )
		{
			int row = index_to_coord( c, _width ).first;

			if( _width % 2 == 1 ) // with odd width, edges are from an odd cell to an even cell.
			{
				if( c % 2 == 0 )
					add_edges_around( c );
			}
			else // else, we have to alternate between even and odd cells for each row
				if( row % 2 == c % 2 )
					add_edges_around( c );				
		}
	}
}

void Graph::add_edges_around( int cell )
{
	int row = index_to_coord( cell, _width ).first;
	int col = index_to_coord( cell, _width ).second;

	if( row - 1 >= 0 )
		add_edge( cell, coord_to_index( row-1, col, _width ) );
	if( row + 1 < _height )
		add_edge( cell, coord_to_index( row+1, col, _width ) );
	if( col - 1 >= 0 )
		add_edge( cell, coord_to_index( row, col-1, _width ) );
	if( col + 1 < _width )
		add_edge( cell, coord_to_index( row, col+1, _width ) );
}

std::vector<std::pair<int,int>> Graph::edges_from_white_cells() const
{
	std::vector<std::pair<int,int>> edges;
	for( auto& white : _white_cells )
		edges.emplace_back( white, _edges[white].front() );
	return edges;
}

void Graph::display() const
{
	for( int c = 0 ; c < _nb_cells ; ++c )
	{
		std::cout << c << ": ";
		for( auto& node: _edges[c] )
			std::cout << node << " ";
		std::cout << std::endl;
	}
}

