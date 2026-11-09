#include <iostream>
#include <algorithm>
#include <map>
#include <stack>
#include <deque>
#include <cassert>
#include <chrono>

#include "edmonds_karp.hpp"
#include "utils.hpp"

/***************
 ** From https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
 ***************/
// For memory usage stats
#include <cstdlib>
#include <cstdio>
#include <cstring>

unsigned long parseLine( char* line )
{
	unsigned long value;
	// This assumes that a digit will be found and the line ends in " Kb".
	int i = strlen( line );
	const char* p = line;
	while( *p <'0' || *p > '9' )
		p++;
	line[ i-3 ] = '\0';
	char* end = nullptr;
	value = strtoul( p, &end, 10 );
	return value;
}

unsigned long get_usage()
{
	FILE* file = fopen("/proc/self/status", "r");
	unsigned long result = 0;
	char line[128];
	
	while( fgets( line, 128, file ) != NULL )
	{
		if( strncmp( line, "VmSize:", 7 ) == 0 )
		{
			result = parseLine( line );
			break;
		}
	}
	fclose( file );
	// Note: this value is in KB!
	return result;
}
/**********************/

using sabrinasgame::EdmondsKarp;

EdmondsKarp::EdmondsKarp( int width, int height )
	: _width(width),
	  _height(height),
	  _residuals(Graph(width,height)),
	  _flows(Graph(width,height,true)),
	  _number_iterations(0),
	  _memory_usage(0),
	  _runtime(0.)
{ }

std::vector<int> EdmondsKarp::random_bfs( const std::vector<int>& path )
{
	Nodepath *current = new Nodepath( path[1] );
	
	std::vector<int> subpath;
	subpath.push_back( path[0] );
	subpath.push_back( path[1] );
	
	std::vector<bool> visited( _residuals.get_nb_nodes(), false );
	visited[current->node] = true;

	std::deque<Nodepath*> queue;
	queue.push_back( current );

	while( !queue.empty() )
	{
		current = queue.front();
		queue.pop_front();

		if( current->node == _residuals.get_sink() )
		{
#if defined DEBUG
			std::cout << "Sink found\n";
#endif
			break;
		}

		auto random_neighbors = _residuals.get_neighbors_of( current->node );
		_rng.shuffle( random_neighbors );

		for( auto neighbor : random_neighbors )
			if( !visited[neighbor] )
			{
#if defined DEBUG
				std::cout << "Visit child " << neighbor << " of " << current->node << "\n";
#endif
				Nodepath *next = new Nodepath( neighbor, current );
				queue.push_back( next );
				visited[neighbor] = true;			
			}
	}

	std::stack<int> stack_path;
	while( current->node != path[1] )
	{
#if defined DEBUG
		std::cout << "Stacking " << current->node << "\n";
#endif
		stack_path.push( current->node );
		current = current->parent;
	}

	while( !stack_path.empty() )
	{
		int node = stack_path.top();
#if defined DEBUG
		std::cout << "Unstacking " << node << "\n";
#endif
		subpath.push_back( node );
		stack_path.pop();
	}
	
	return subpath;
}

sabrinasgame::Graph EdmondsKarp::solve()
{
	std::chrono::time_point<std::chrono::steady_clock> start_wall_clock( std::chrono::steady_clock::now() );
	std::chrono::duration<double,std::micro> elapsed_time( 0 );

	std::vector<int> path;
	
	std::map<int, int> black_node_visits;
	for( auto black_node : _residuals.get_black_nodes() )
		black_node_visits[black_node] = 0;

	auto white_nodes = _residuals.get_white_nodes();
	auto black_nodes = _residuals.get_black_nodes();	
	
	while( !_residuals.get_neighbors_of( _residuals.get_source() ).empty() )
	{
		++_number_iterations;
		
#if defined DEBUG
		std::cout << "\nIteration " << _number_iterations << "\n";
#endif
		path.clear();	
		_residual_outgoing_edges = _residuals.get_outgoing_edges();
		_residual_incoming_edges = _residuals.get_incoming_edges();

		path.push_back( _residuals.get_source() );

		int current = _rng.pick( _residual_outgoing_edges[ _residuals.get_source() ] );
		path.push_back( current );
#if defined DEBUG
		std::cout << "Current: " << current << "\n";
#endif

		path = random_bfs( path );
		
#if defined DEBUG
		for( auto& elem : path )
			std::cout << elem << " ";
		std::cout << std::endl;
#endif
		
		for( int i = 0 ; i < path.size() - 1 ; ++i )
		{
			_flows.remove_edge( path[i+1], path[i] ); // remove if it exists
			_flows.add_edge( path[i], path[i+1] );
			
			_residuals.remove_edge( path[i], path[i+1] );
#if defined DEBUG
			std::cout << "Remove (" << path[i] << ", " << path[i+1] << ")\n";
#endif
			if( path[i] != _residuals.get_source() && path[i+1] != _residuals.get_sink() )
			{
				_residuals.add_edge( path[i+1], path[i] );
#if defined DEBUG
				std::cout << "Add (" << path[i+1] << ", " << path[i] << ")\n";
#endif
			}
		}		
	}

	elapsed_time = std::chrono::steady_clock::now() - start_wall_clock;
	_runtime = elapsed_time.count();
	_memory_usage = get_usage();
	
	return _flows;
}

void EdmondsKarp::print() const
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
