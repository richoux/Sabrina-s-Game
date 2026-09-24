#include <iostream>
#include <algorithm>

#include "graph.hpp"
#include "utils.hpp"

using sabrinasgame::Graph;

Graph::Graph( int width, int height, bool empty )
	: _width(width),
	  _height(height),
	  _nb_nodes(width * height + 2), // +2 because we add a source and a sink
	  _source(_nb_nodes-2),
	  _sink(_nb_nodes-1),
	  _outgoing_edges(std::vector<std::deque<int>>(_nb_nodes)),
	  _incoming_edges(std::vector<std::deque<int>>(_nb_nodes)),
	  _white_nodes(std::vector<int>()),
	  _black_nodes(std::vector<int>())
{
	for( int c = 0 ; c < _nb_nodes - 2 ; ++c )
	{
		int row = index_to_coord( c, _width ).first;
		
		if( _width % 2 == 1 )
		{
			if( c % 2 == 0 )
				_white_nodes.push_back( c );
			else
				_black_nodes.push_back( c );
		}
		else
		{
			if( row % 2 == c % 2 )
				_white_nodes.push_back( c );
			else
				_black_nodes.push_back( c );
		}
	}

	if( !empty )
	{
		for( auto& white : _white_nodes )
			add_edge( _source, white );

		for( auto& black : _black_nodes )
			add_edge( black, _sink );

		for( int c = 0 ; c < _nb_nodes - 2 ; ++c )
		{
			int row = index_to_coord( c, _width ).first;

			if( _width % 2 == 1 ) // with odd width, edges are from an odd node to an even node.
			{
				if( c % 2 == 0 )
					add_edges_around( c );
			}
			else // else, we have to alternate between even and odd nodes for each row
				if( row % 2 == c % 2 )
					add_edges_around( c );				
		}
	}
}

void Graph::add_edges_around( int node )
{
	int row = index_to_coord( node, _width ).first;
	int col = index_to_coord( node, _width ).second;

	if( row - 1 >= 0 )
		add_edge( node, coord_to_index( row-1, col, _width ) );
	if( row + 1 < _height )
		add_edge( node, coord_to_index( row+1, col, _width ) );
	if( col - 1 >= 0 )
		add_edge( node, coord_to_index( row, col-1, _width ) );
	if( col + 1 < _width )
		add_edge( node, coord_to_index( row, col+1, _width ) );
}

void Graph::add_edge( int node1, int node2 )
{
	_outgoing_edges[node1].push_back( node2 );
	_incoming_edges[node2].push_back( node1 );
}

void Graph::remove_edge( int node1, int node2 )
{
	std::erase(	_outgoing_edges[node1], node2 );
	std::erase(	_incoming_edges[node2], node1 );
}


std::vector<std::pair<int,int>> Graph::edges_from_white_nodes() const
{
	std::vector<std::pair<int,int>> edges;
	for( auto& white : _white_nodes )
		edges.emplace_back( white, _outgoing_edges[white].front() );
	return edges;
}

// std::deque<int> Graph::get_excluding_neighbors_of( int node, const std::vector<int>& to_exclude ) const
// {
// 	auto edges = _outgoing_edges[node];
// 	for( int i = 1 ; i < to_exclude.size() ; ++i ) // keep the source
// 		std::erase(	edges, to_exclude[i] );
// 	return edges;
// }

// std::deque<int> Graph::get_unvisited_neighbors_of( int node,
//                                                    const std::vector<int>& to_exclude,
//                                                    const std::map<int,int>& visits ) const
// {
// 	std::deque<int> returned_edges;
// 	auto prefiltered_edges = get_excluding_neighbors_of( node, to_exclude );
	
// 	int min_visits = std::numeric_limits<int>::max();
// 	for( auto visit : visits )
// 		if( std::find( prefiltered_edges.begin(), prefiltered_edges.end(), visit.first ) != prefiltered_edges.end() && visit.second < min_visits )
// 			min_visits = visit.second;

// 	std::cout << "Visits: ";
// 	for( auto v : visits )
// 		std::cout << "visits[" << v.first << "]=" << v.second << " ";
// 	std::cout << std::endl;
	
// 	for( auto edge : prefiltered_edges )
// 		if( !visits.contains( edge ) || visits.at(edge) == min_visits ) // accepts "unvisited" nodes, like white ones
// 			returned_edges.push_back( edge );

// 	std::cout << "Returned edges from " << node << ": ";
// 	for( auto edge : returned_edges )
// 		std::cout << edge << " ";	
// 	std::cout << std::endl;
	
// 	return returned_edges;
// }

void Graph::display() const
{
	for( int c = 0 ; c < _nb_nodes ; ++c )
	{
		std::cout << c << ": ";
		for( auto& node: _outgoing_edges[c] )
			std::cout << node << " ";
		std::cout << std::endl;
	}
}

