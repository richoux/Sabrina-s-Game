#include <iostream>
#include <algorithm>
#include <map>
#include <cassert>

#include "ford_fulkerson.hpp"
#include "utils.hpp"

using sabrinasgame::FordFulkerson;

FordFulkerson::FordFulkerson( int width, int height )
	: _width(width),
	  _height(height),
	  _residuals(Graph(width,height)),
	  _flows(Graph(width,height,true))
{ }

void FordFulkerson::recursive_remove_outgoing_edges( int node, int parent )
{
	if( !_residual_outgoing_edges[parent].empty() && node != _residuals.get_sink() )
	{
		std::cout << "Remove " << node << " from the neighbors of " << parent << "\n";
		std::erase(	_residual_outgoing_edges[parent], node );
		
		if( _residual_outgoing_edges[parent].empty() )
		{
			std::cout << "No neighbors anymore!\n";
			for( auto greatparent : _residual_incoming_edges[parent] )
				recursive_remove_outgoing_edges( parent, greatparent );
		}
		std::cout << "End of recursive call (" << node << ", " << parent << ")\n";
	}
}

void FordFulkerson::filtering_neighbors_of( int node,
                                            bool is_white,
                                            const std::vector<int>& path_to_exclude,
                                            const std::map<int,int>& visits )
{
	for( int i = 1 ; i < path_to_exclude.size() ; ++i ) // keep the source
	{
		std::erase(	_residual_outgoing_edges[node], path_to_exclude[i] );
		std::erase(	_residual_incoming_edges[path_to_exclude[i]], node );
	}

	if( is_white )
	{
		int min_visits = std::numeric_limits<int>::max();
		for( auto visit : visits )
			if( std::find( _residual_outgoing_edges[node].begin(), _residual_outgoing_edges[node].end(), visit.first ) != _residual_outgoing_edges[node].end()
			    && visit.second < min_visits )
			{
				min_visits = visit.second;
			}
		
		std::cout << "Visits: ";
		for( auto v : visits )
		{
			std::cout << "visits[" << v.first << "]=" << v.second << " ";
			if( std::find( _residual_outgoing_edges[ node ].begin(),
			               _residual_outgoing_edges[ node ].end(),
			               v.first ) != _residual_outgoing_edges[ node ].end()
			    && v.second > min_visits )
			{
				std::erase(	_residual_outgoing_edges[node], v.first );
				std::erase(	_residual_incoming_edges[v.first], node );
			}
		}
		std::cout << std::endl;
	}
}

sabrinasgame::Graph FordFulkerson::solve()
{
	std::vector<int> path;
	int count = 1;

	std::map<int, int> black_node_visits;
	for( auto black_node : _residuals.get_black_nodes() )
		black_node_visits[black_node] = 0;

	std::map<int, int> white_node_visits;
	for( auto white_node : _residuals.get_white_nodes() )
		white_node_visits[white_node] = 0;

	auto white_nodes = _residuals.get_white_nodes();
	auto black_nodes = _residuals.get_black_nodes();	
	bool is_white_node;
	
	while( !_residuals.get_neighbors_of( _residuals.get_source() ).empty() )
	{
		std::cout << "\nIteration " << count++ << "\n";
		path.clear();	
		_residual_outgoing_edges = _residuals.get_outgoing_edges();
		_residual_incoming_edges = _residuals.get_incoming_edges();

		path.push_back( _residuals.get_source() );

		int current = _rng.pick( _residual_outgoing_edges[ _residuals.get_source() ] );
		int next;
		path.push_back( current );
		std::cout << "Current: " << current << "\n";
		
		while( current != _residuals.get_sink() )
		{
			std::cout << "New loop\n";
			is_white_node = std::find( white_nodes.begin(),
			                           white_nodes.end(),
			                           current ) != white_nodes.end();
			std::cout << "Current is " << ( is_white_node ? "white\n" : "black\n" );
			
			filtering_neighbors_of( current, is_white_node, path, black_node_visits );
			std::cout << "Current " << current << " filtered neighbors: ";
			for( auto neighbor: _residual_outgoing_edges[current] )
				std::cout << neighbor << " ";
			std::cout << std::endl;
			assert( !_residual_outgoing_edges[current].empty() );

			
			if( std::find( _residual_outgoing_edges[ current ].begin(),
			               _residual_outgoing_edges[ current ].end(),
			               _residuals.get_sink() ) != _residual_outgoing_edges[ current ].end() )
			{
				next = _residuals.get_sink();
			}
			else
			{
				// heuristics to favor nodes near the "exit" if all neighbors of the current white node have been visited.
				int unvisited_node = -1;
				if( is_white_node )
				{
					for( auto neighbor : _residual_outgoing_edges[current] )
						if( black_node_visits[ neighbor ] == 0 )
						{
							unvisited_node = neighbor;
							break;
						}
				}
				
				if( is_white_node && unvisited_node == -1 )
				{
					// find the closest unvisited node among black nodes, not just neighbors
					int min_distance = std::numeric_limits<int>::max();
					for( auto black_node : black_nodes )
						if( black_node_visits[ black_node] == 0 )
							if( min_distance > std::abs( current - black_node ) )
							{
								min_distance = std::abs( current - black_node );
								unvisited_node = black_node;									
							}
										
					int shortest_gap = std::numeric_limits<int>::max();
					std::vector<int> candidate_neighbors;
					for( auto edge : _residual_outgoing_edges[ current ] )
						if( std::abs( edge - unvisited_node ) <= shortest_gap )
						{
							if( std::abs( edge - unvisited_node ) < shortest_gap )
							{
								shortest_gap = std::abs( edge - unvisited_node );
								candidate_neighbors.clear();
							}
							candidate_neighbors.push_back( edge );								
						}
					
					next = _rng.pick( candidate_neighbors );
				}
				else				
					next = _rng.pick( _residual_outgoing_edges[ current ] );
			}
			
			path.push_back( next );

			if( is_white_node )
				++black_node_visits[next];

			for( auto parent : _residual_incoming_edges[next] )
				recursive_remove_outgoing_edges( next, parent );
			
			std::cout << next << " is in the path\n";
			current = next;
		}

		std::vector<int> short_path;
		short_path.push_back( _residuals.get_source() );
		std::copy( std::find( path.rbegin(), path.rend(), _residuals.get_source() ).base(), path.end(), std::back_inserter( short_path ) );

		for( auto& elem : short_path )
			std::cout << elem << " ";
		std::cout << std::endl;
		
		for( int i = 0 ; i < short_path.size() - 1 ; ++i )
		{
			_flows.remove_edge( short_path[i+1], short_path[i] ); // remove if it exists
			_flows.add_edge( short_path[i], short_path[i+1] );
			
			_residuals.remove_edge( short_path[i], short_path[i+1] );
			std::cout << "Remove (" << short_path[i] << ", " << short_path[i+1] << ")\n";
			if( short_path[i] != _residuals.get_source() && short_path[i+1] != _residuals.get_sink() )
			{
				_residuals.add_edge( short_path[i+1], short_path[i] );
				std::cout << "Add (" << short_path[i+1] << ", " << short_path[i] << ")\n";
			}
		}
	}
	
	return _flows;
}

void FordFulkerson::print() const
{
	// "┌ ┐ └ ┘ │ ┤ ├ ─ ┴ ┬ ┼"

	std::vector<std::vector<int>> done(_height, std::vector<int>());
	for( int row = 0 ; row < _height ; ++row )
		done[row] = std::vector<int>(_width, 0);
	std::vector<int> first_line(_width, 0);
	
	for( auto &edge: _flows.edges_from_white_nodes() )
	{
		auto c1 = index_to_coord(edge.first, _width);
		auto c2 = index_to_coord(edge.second, _width);

		if( c1.first > c2.first || c1.second > c2.second)
			std::swap( c1, c2 );
		
		bool is_vertical = (c1.second == c2.second);
		if( done[c1.first][c1.second] == 0 )
		{
			if(is_vertical)
			{
				done[c1.first][c1.second] = -1;
				done[c2.first][c2.second] = -2;				
			}
			else
			{
				done[c1.first][c1.second] = -3;
				done[c2.first][c2.second] = -4;
			}
		}
	}

	for(int j = 0 ; j < _height ; ++j )
		for( int i = 0 ; i < _width ; ++i )
		{
			if( j == 0 )
				first_line[i] = done[j][i];

			switch( done[j][i] )
			{
			case -1: // first vertical
				if( i == _width - 1 )
					done[j][i] = 2;
				else
					if( done[j][i+1] == -1 ) // first vertical
						done[j][i] = 1;
					else
						done[j][i] = 3;
				break;
			case -3: // first horizontal
				if( j == _height - 1 )
					done[j][i] = 11;
				else
					if( done[j+1][i] == -1 || done[j+1][i] == -4 )
						done[j][i] = 4;
					else
						done[j][i] = 11;
				break;
			default: // second 
				if( i == _width - 1 )
					if( j == _height - 1 )
						done[j][i] = 10;
					else
						done[j][i] = 8;
				else
					if( done[j][i+1] == -1 ) // first vertical
						done[j][i] = 7;
					else
						if( j == _height - 1 )
							done[j][i] = 9;
						else
							if( done[j+1][i] == -3 ) // first horizontal
								done[j][i] = 6;
							else
								done[j][i] = 5;
				break;				
			}
		}
	
	std::cout << "\n┌";
	for( int i = 0 ; i < _width - 1 ; ++i )
		if( first_line[i] == -1 || first_line[i] == -4 )
			std::cout << "──┬";
		else
			std::cout << "───";
	std::cout << "──┐\n";

	for(int j = 0 ; j < _height ; ++j )
		for( int i = 0 ; i < _width ; ++i )
		{
			if( i == 0 )
			{
				if( j == _height - 1 )
					std::cout << "└";
				else
					if( done[j][0] > 3)
						std::cout << "├";
					else
						std::cout << "│";
			}

			switch(done[j][i])
			{
			case 1:
				std::cout << "  │";
				break;
			case 2:
				std::cout << "  │\n";
				break;
			case 3:
				std::cout << "  ├";
				break;
			case 4:
				std::cout << "──┬";
				break;
			case 5:
				std::cout << "──┼";
				break;
			case 6:
				std::cout << "──┴";
				break;
			case 7:
				std::cout << "──┤";
				break;
			case 8:
				std::cout << "──┤\n";
				break;
			case 9:
				std::cout << "──┴";
				break;
			case 10:
				std::cout << "──┘\n";
				break;
			default:
				std::cout << "───";
			}
		}
}
