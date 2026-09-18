#include <iostream>
#include <algorithm>
#include <map>

#include "ford_fulkerson.hpp"
#include "utils.hpp"

using sabrinasgame::FordFulkerson;

FordFulkerson::FordFulkerson( int width, int height )
	: _width(width),
	  _height(height),
	  _residuals(Graph(width,height)),
	  _flows(Graph(width,height,true))
{}

sabrinasgame::Graph FordFulkerson::solve()
{
	std::vector<int> path;
	int count = 1;

	std::map<int, int> black_cell_visits;
	for( auto black_cell : _residuals.get_black_cells() )
		black_cell_visits[black_cell] = 0;
	
	while( !_residuals.get_cells_from( _residuals.get_source() ).empty() )
	{
		std::cout << "\nIteration " << count++ << "\n";
		path.clear();	

		path.push_back( _residuals.get_source() );

		int current = _rng.pick( _residuals.get_cells_from( _residuals.get_source() ) );
		int next;
		// int previous_next = -1;
		// int count_next = 0;
		path.push_back( current );
		std::cout << "Current: " << current << "\n";
		
		while( current != _residuals.get_sink() )
		{
			const auto& neighbors = _residuals.get_filtered_cells_from( current, path, black_cell_visits );
			if( std::find( neighbors.begin(), neighbors.end(), _residuals.get_sink() ) != neighbors.end() )
				next = _residuals.get_sink();
			else
				do
				{
					next = _rng.pick( neighbors );
					// if( next == previous_next )
					// 	++count_next;
					// else
					// 	count_next = 0;

					// if( count_next > 5 )
					// {
					// 	std::cout << "COMBO BREAKER!!\n";
					// 	break;
					// }
					// previous_next = next;					
					std::cout << "Next: " << next << "\n";
				}
				while( std::find( path.begin(), path.end(), next ) != path.end() && next != _residuals.get_source() ); // next should be a node that is not already in the path, except if it is the source 
			path.push_back( next );

			if( black_cell_visits.contains(next) )
				++black_cell_visits[next];

			std::cout << next << " is in the path\n";
			current = next;
			// if( count_next > 5 )
			// 	break;
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
	
	for( auto &edge: _flows.edges_from_white_cells() )
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
