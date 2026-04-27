#include <ctime>
#include <cassert>

#include "build.hpp"
#include "utils.hpp"

using sabrinasgame::Solution;

Solution::Solution( int width, int height )
	: _width(width),
		_height(height),
		_grid(std::vector<std::vector<int>>(height)),
		_gen(std::mt19937(_rd())),
		_distrib(std::uniform_int_distribution<>(0,1))
{
	for( int h = 0 ; h < _height ; ++h )
		_grid[ h ] = std::vector<int>( _width );
}

bool Solution::toss_coin()
{
	return _distrib( _gen ) == 0;
}

void Solution::init_grid()
{
	_grid[0][0] = 2;
	_grid[0][_width - 1] = 2;
	_grid[_height - 1][0] = 2;
	_grid[_height - 1][_width - 1] = 2;

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

void Solution::fill( int row, int col )
{
	//std::cout << "Filling (" << row << "," << col << ") " << _grid[row][col] << "\n";
	assert(_grid[row][col] != -1);
	
	--_count[_grid[row][col]];
	_grid[row][col] = -1;
	
	assert(_count[0] == 0);
}

void Solution::decrease( int row, int col )
{
	//std::cout << "Decreasing (" << row << "," << col << ") " << _grid[row][col] << "\n";
	assert(_grid[row][col] != -1);

	--_count[_grid[row][col]];
	--_grid[row][col];
	++_count[_grid[row][col]];

	assert(_count[0] == 0);

	//std::cout << "_grid[2][0]=" << _grid[2][0] << ")\n";
}

void Solution::decrease_around( int row1, int col1, int row2, int col2 )
{
	//std::cout << "Begin decrease_around\n";

	if( row1 > row2 )
		std::swap( row1, row2 );
	if( col1 > col2 )
		std::swap( col1, col2 );

	//std::cout << row1 << "," << col1 << " " << row2 << "," << col2 << "\n";

	bool is_vertical = ( col1 == col2 );
	bool trigger_forced_move = false;
	
	//std::cout << "is_vertical=" << is_vertical << "\n";

	if( col1 - 1 >= 0 && _grid[row1][col1 - 1] > 0 )
	{
		//std::cout << "_grid["<< row1 << "]["<< col1 - 1 << "]="<< _grid[row1][col1 - 1] << "\n";
		decrease( row1, col1 - 1 );
		//std::cout << "W\n";
	}
	if( col2 + 1 < _width && _grid[row2][col2 + 1] > 0 )
	{
		//std::cout << "_grid["<< row2 << "]["<< col2+1 << "]="<< _grid[row2][col2+1] << "\n";
		decrease( row2, col2 + 1 );
		//std::cout << "E\n";
	}
	if( row1 - 1 >= 0 && _grid[row1 - 1][col1] > 0 )
	{
		//std::cout << "_grid["<< row1-1 << "]["<< col1 << "]="<< _grid[row1-1][col1] << "\n";
		decrease( row1 - 1, col1 );
		//std::cout << "N\n";
	}
	if( row2 + 1 < _height && _grid[row2 + 1][col2] > 0 )
	{
		//std::cout << "_grid["<< row2+1 << "]["<< col2 << "]="<< _grid[row2+1][col2] << "\n";
		decrease( row2 + 1, col2);
		if( is_vertical && row2 == _height - 2 && col2 > 1 && col2 < _width - 2 )
			trigger_forced_move = true;
		//std::cout << "S\n";
	}

	//std::cout << "Common decrease_around\n";

	if( is_vertical )
	{
		if( col2 - 1 >= 0 && _grid[row2][col2 - 1] > 0 )
		{
			//std::cout << "_grid["<< row2 << "]["<< col2-1 << "]="<< _grid[row2][col2-1] << "\n";
			decrease( row2, col2 - 1 );
			//std::cout << "SW\n";
		}
		if( col1 + 1 < _width && _grid[row1][col1 + 1] > 0 )
		{
			//std::cout << "_grid["<< row1 << "]["<< col1+1 << "]="<< _grid[row1][col1+1] << "\n";
			decrease( row1, col1 + 1 );
			//std::cout << "NE\n";
		}
		//std::cout << "Vertical decrease_around\n";
	}
	else // horizontal
	{
		if( row2 - 1 >= 0 && _grid[row2 - 1][col2] > 0 )
		{
			//std::cout << "decrease _grid["<< row2-1 << "]["<< col2 << "]="<< _grid[row2-1][col2] << "\n";
			decrease( row2 - 1, col2 );
			//std::cout << "NE\n";
		}
		if( row1 + 1 < _height && _grid[row1 + 1][col1] > 0 )
		{
			//std::cout << "_grid["<< row1+1 << "]["<< col1 << "]="<< _grid[row1+1][col1] << "\n";
			decrease( row1 + 1, col1);
			//std::cout << "SW\n";
		}
		//std::cout << "Horizontal decrease_around\n";
	}

	if( trigger_forced_move )
	{
		if( left_scan_is_even( col2 ) )
			force_move( row2 + 1, col2, row2 + 1, col2 + 1 );
		else
			force_move( row2 + 1, col2 - 1, row2 + 1, col2 );
	}
	
	//std::cout << "End decrease_around\n";
}

void Solution::greedy_fill( int index )
{
	// find the first cell (row,col) in the grid such that _grid[row][col] = -1
	std::pair<int,int> coord;
	for( ; index < _height * _width ; ++index )
	{
		coord = index_to_coordinates( index );
		if( _grid[coord.first][coord.second] == 1 )
			break;		
	}

	int row = coord.first;
	int col = coord.second;

	if( col - 1 >= 0 && _grid[row][col - 1] > 0 ) // West
	{
		fill( row, col );
		fill( row, col - 1 );
		//std::cout << "W-Filling ([" << row << "][" << col -1<< "]), ([" << row << "][" << col << "])\n";
		decrease_around( row, col - 1, row, col );
		_built_solution.emplace_back( coordinates_to_index( row, col - 1 ), coordinates_to_index( row, col ) );
		return;
	}
	if( row - 1 >= 0 && _grid[row - 1][col] > 0 ) // North
	{
		fill( row, col );
		fill( row - 1, col );
		//std::cout << "N-Filling ([" << row -1<< "][" << col << "]), ([" << row << "][" << col << "])\n";
		decrease_around( row - 1, col, row, col );
		_built_solution.emplace_back( coordinates_to_index( row - 1, col ), coordinates_to_index( row, col ) );
		return;
	}
	if( col + 1 < _width && _grid[row][col + 1] > 0 ) // East
	{
		fill( row, col );
		fill( row, col + 1 );
		//std::cout << "E-Filling ([" << row << "][" << col << "]), ([" << row << "][" << col+1 << "])\n";
		decrease_around( row, col, row, col + 1 );
		_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row, col + 1 ) );
		return;
	}
	if( row + 1 < _height && _grid[row + 1][col] > 0 ) // South
	{
		fill( row, col );
		fill( row + 1, col );
		//std::cout << "S-Filling ([" << row << "][" << col << "]), ([" << row+1 << "][" << col << "])\n";
		decrease_around( row, col, row + 1, col );
		_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row + 1, col ) );
	}
}

void Solution::force_move( int r1, int c1, int r2, int c2 )
{
	//std::cout << "Force " << r1 << ","<< c1 << " " << r2 << ","<< c2 << "\n";				
	fill( r1, c1 );
	fill( r2, c2 );
	_built_solution.emplace_back( coordinates_to_index( r1, c1 ), coordinates_to_index( r2, c2 ) );
	if( _grid[r1][c1-1] != -1 )
		decrease( r1, c1 - 1 );
	if( _grid[r2][c2+1] != -1 )
		decrease( r2, c2 + 1 );
	if( _grid[r1-1][c1] != -1 )
		decrease( r1 - 1, c1 );
	if( _grid[r2-1][c2] != -1 )
		decrease( r2 - 1, c2 );
}

bool Solution::left_scan_is_even( int col ) const
{
	int filled = 0;
	for( int r = 0 ; r < _height ; ++r )
		for( int c = 0 ; c < col ; ++c )
			if( _grid[r][c] != -1 )
				++filled;

	return filled % 2 == 0;
}

std::vector< std::pair<int,int> > Solution::build()
{
	init_grid();
	
	for( int row = 0 ; row < _height ; ++row )
	{
		for( int col = 0 ; col < _width ; ++col )
		{
			//std::cout << "Looking at _grid[" << row << "]["<< col<< "]\n";				
			if( _grid[row][col] != -1 )
			{
				//std::cout << "Unfilled cell\n";
				fill( row, col );
				if( toss_coin() ) //horizontal tuple
				{
					//std::cout << "Horizontal\n";
					fill( row, col + 1 );
					_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row, col + 1 ) );
					
					if( col + 2 < _width && _grid[row][col+2] != -1 )
						decrease( row, col + 2 );
					if( row + 1 < _height )
					{
						if( _grid[row+1][col] != -1 )
							decrease( row + 1, col );
						if( _grid[row+1][col+1] != -1 )
							decrease( row + 1, col + 1 );
					}
				}
				else //vertical tuple
				{
					//std::cout << "Vertical _grid[" << row+1 << "][" << col << "]=" << _grid[row+1][col] << "\n";
					fill( row + 1, col );
					_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row + 1, col ) );

					if( row + 2 < _height && _grid[row+2][col] != -1 )
						decrease( row + 2, col );
					if( col + 1 < _width )
					{
						if( _grid[row][col+1] != -1 )
							decrease( row, col + 1 );
						if( _grid[row+1][col+1] != -1 )
							decrease( row + 1, col + 1 );
					}
					if( col - 1 >= 0 && _grid[row+1][col-1] != -1 )
						decrease( row + 1, col - 1 );
					if( row == _height - 3 && _grid[row+2][col] != -1 && col > 1 && col < _width - 2 )
					{
						if( left_scan_is_even( col ) )
							force_move( row + 2, col, row + 2, col + 1 );
						else
							force_move( row + 2, col - 1, row + 2, col );
					}
				}
				// for( int a = 0 ; a <= 4 ; ++a )
				// 	std::cout << "_count[" << a << "]=" << _count[a] << "\n";
				while( _count[1] > 0 )
				{
					greedy_fill( coordinates_to_index( row, col ) );
					// for( int a = 0 ; a <= 4 ; ++a )
					//  	std::cout << "_count[" << a << "]=" << _count[a] << "\n";
				}
			}
		}
	}

	return _built_solution;
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

