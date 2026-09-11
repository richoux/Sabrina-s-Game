#include <ctime>

#include "solution.hpp"
#include "utils.hpp"

using sabrinasgame::Solution;

Solution::Solution( int width, int height )
	: _width(width),
		_height(height),
		_grid(std::vector<std::vector<int>>(height))
{
	for( int h = 0 ; h < _height ; ++h )
		_grid[ h ] = std::vector<int>( _width );
}

bool Solution::toss_coin()
{
	return _rng.pick({true,false});
}

void Solution::init_grid()
{
	_is_valid = false;
	
	_grid[0][0] = 2;
	_grid[0][_width - 1] = 2;
	_grid[_height - 1][0] = 2;
	_grid[_height - 1][_width - 1] = 2;

	_list_count2.push_back( coordinates_to_index( 0, 0 ) );
	_list_count2.push_back( coordinates_to_index( 0, _width - 1 ) );
	_list_count2.push_back( coordinates_to_index( _height - 1, 0 ) );
	_list_count2.push_back( coordinates_to_index( _height - 1, _width - 1 ) );
	
	for( int row = 1 ; row < _height - 1 ; ++row )
	{
		_grid[row][0] = 3;
		_grid[row][_width - 1] = 3;
	}

	for( int col = 1 ; col < _width - 1 ; ++col )
	{
		_grid[0][col] = 3;
		_grid[_height - 1][col] = 3;
	}

	for( int row = 1 ; row < _height  - 1 ; ++row )
		for( int col = 1 ; col < _width - 1 ; ++col )
			_grid[row][col] = 4;

	_built_solution.clear();
	
	_count[0] = 0;
	_count[1] = 0;
	_count[2] = 4;
	_count[3] = _width * 2 + _height * 2 - 8;
	_count[4] = (_width - 2) * (_height - 2);
}


void Solution::print() const
{
	// "┌ ┐ └ ┘ │ ┤ ├ ─ ┴ ┬ ┼"

	std::vector<std::vector<int>> done(_height, std::vector<int>());
	for( int row = 0 ; row < _height ; ++row )
		done[row] = std::vector<int>(_width, 0);
	std::vector<int> first_line(_width, 0);
	
	for( auto &pair_of_index: _built_solution )
	{
		auto c1 = index_to_coordinates(pair_of_index.first);
		auto c2 = index_to_coordinates(pair_of_index.second);

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

std::pair<int,int> Solution::index_to_coordinates( int index ) const
{
	return index_to_coord( index, _width );
}

int Solution::coordinates_to_index( int first, int second ) const
{
	return coord_to_index( first, second, _width );
}

int Solution::coordinates_to_index( const std::pair<int,int>& coord ) const
{
	return coord_to_index( coord.first, coord.second, _width );
}

