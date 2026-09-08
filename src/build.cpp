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
	_is_valid = false;
	
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

bool Solution::fill( int row, int col )
{
#if defined DEBUG
	std::cout << "Filling (" << row << "," << col << ") " << _grid[row][col] << "\n";
	assert(_grid[row][col] != -1);
#endif
	if( _grid[row][col] == -1 )
		return false;
	
	--_count[_grid[row][col]];
	_grid[row][col] = -1;
	
#if defined DEBUG
	assert(_count[0] == 0);
#endif
	return _count[0] == 0;
}

bool Solution::decrease( int row, int col )
{
#if defined DEBUG
	std::cout << "Decreasing (" << row << "," << col << ") " << _grid[row][col] << "\n";
	assert(_grid[row][col] != -1);
#endif
	if( _grid[row][col] == -1 )
		return false;

	--_count[_grid[row][col]];
	--_grid[row][col];
	++_count[_grid[row][col]];

#if defined DEBUG
	assert(_count[0] == 0);
	std::cout << "new value (" << row << "," << col << ") " << _grid[row][col] << "\n";
#endif
	return _count[0] == 0;
}

bool Solution::decrease_around( int row1, int col1, int row2, int col2 )
{
#if defined DEBUG
	std::cout << "Begin decrease_around\n";
#endif
	
	if( row1 > row2 )
		std::swap( row1, row2 );
	if( col1 > col2 )
		std::swap( col1, col2 );

#if defined DEBUG
	std::cout << row1 << "," << col1 << " " << row2 << "," << col2 << "\n";
#endif
	
	bool is_vertical = ( col1 == col2 );
#if defined V2
	bool trigger_forced_move = false;
#endif
	
#if defined DEBUG
	std::cout << "is_vertical=" << is_vertical << "\n";
#endif
	
	if( col1 - 1 >= 0 && _grid[row1][col1 - 1] > 0 )
	{
#if defined DEBUG
		std::cout << "_grid["<< row1 << "]["<< col1 - 1 << "]="<< _grid[row1][col1 - 1] << "\n";
#endif
		if( !decrease( row1, col1 - 1 ) )
			return false;
#if defined DEBUG
		std::cout << "W\n";
#endif
	}
	if( col2 + 1 < _width && _grid[row2][col2 + 1] > 0 )
	{
#if defined DEBUG
		std::cout << "_grid["<< row2 << "]["<< col2+1 << "]="<< _grid[row2][col2+1] << "\n";
#endif
		if( !decrease( row2, col2 + 1 ) )
			return false;
#if defined DEBUG
		std::cout << "E\n";
#endif
	}
	if( row1 - 1 >= 0 && _grid[row1 - 1][col1] > 0 )
	{
#if defined DEBUG
		std::cout << "_grid["<< row1-1 << "]["<< col1 << "]="<< _grid[row1-1][col1] << "\n";
#endif
		if( !decrease( row1 - 1, col1 ) )
			return false;
#if defined DEBUG
		std::cout << "N\n";
#endif
	}
	if( row2 + 1 < _height && _grid[row2 + 1][col2] > 0 )
	{
#if defined DEBUG
		std::cout << "_grid["<< row2+1 << "]["<< col2 << "]="<< _grid[row2+1][col2] << "\n";
#endif
		if( !decrease( row2 + 1, col2) )
			return false;
#if defined V2
		if( is_vertical && row2 == _height - 2 && col2 > 1 && col2 < _width - 2 )
			trigger_forced_move = true;
#if defined DEBUG
		std::cout << "S\n";
#endif
#endif
	}

#if defined DEBUG
	std::cout << "Common decrease_around\n";
#endif

	if( is_vertical )
	{
		if( col2 - 1 >= 0 && _grid[row2][col2 - 1] > 0 )
		{
#if defined DEBUG
			std::cout << "_grid["<< row2 << "]["<< col2-1 << "]="<< _grid[row2][col2-1] << "\n";
#endif
			if( !decrease( row2, col2 - 1 ) )
				return false;
#if defined DEBUG
			std::cout << "SW\n";
#endif
		}
		if( col1 + 1 < _width && _grid[row1][col1 + 1] > 0 )
		{
#if defined DEBUG
			std::cout << "_grid["<< row1 << "]["<< col1+1 << "]="<< _grid[row1][col1+1] << "\n";
#endif
			if( !decrease( row1, col1 + 1 ) )
				return false;
#if defined DEBUG
			std::cout << "NE\n";
#endif
		}
#if defined DEBUG
		std::cout << "Vertical decrease_around\n";
#endif
	}
	else // horizontal
	{
		if( row2 - 1 >= 0 && _grid[row2 - 1][col2] > 0 )
		{
#if defined DEBUG
			std::cout << "decrease _grid["<< row2-1 << "]["<< col2 << "]="<< _grid[row2-1][col2] << "\n";
#endif
			if( !decrease( row2 - 1, col2 ) )
				return false;
#if defined DEBUG
			std::cout << "NE\n";
#endif
		}
		if( row1 + 1 < _height && _grid[row1 + 1][col1] > 0 )
		{
#if defined DEBUG
			std::cout << "_grid["<< row1+1 << "]["<< col1 << "]="<< _grid[row1+1][col1] << "\n";
#endif
			if( !decrease( row1 + 1, col1) )
				return false;
#if defined DEBUG
			std::cout << "SW\n";
#endif
		}
#if defined DEBUG
		std::cout << "Horizontal decrease_around\n";
#endif
	}

#if defined V2
	if( trigger_forced_move )
	{
		if( left_scan_is_even( col2 ) )
			force_move( row2 + 1, col2, row2 + 1, col2 + 1 );
		else
			force_move( row2 + 1, col2 - 1, row2 + 1, col2 );
	}
#endif
	
#if defined DEBUG
	std::cout << "End decrease_around\n";
#endif
	return true;
}

bool Solution::greedy_fill( int index )
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
	bool no_errors = true;
	
	if( col - 1 >= 0 && _grid[row][col - 1] > 0 ) // West
	{
		no_errors = no_errors && fill( row, col );
		no_errors = no_errors && fill( row, col - 1 );
#if defined DEBUG
		std::cout << "W-Filling ([" << row << "][" << col -1<< "]), ([" << row << "][" << col << "])\n";
#endif
		no_errors = no_errors && decrease_around( row, col - 1, row, col );

		if( no_errors )
			_built_solution.emplace_back( coordinates_to_index( row, col - 1 ), coordinates_to_index( row, col ) );
		return no_errors;
	}
	if( row - 1 >= 0 && _grid[row - 1][col] > 0 ) // North
	{
		no_errors = no_errors && fill( row, col );
		no_errors = no_errors && fill( row - 1, col );
#if defined DEBUG
		std::cout << "N-Filling ([" << row -1<< "][" << col << "]), ([" << row << "][" << col << "])\n";
#endif
		no_errors = no_errors && decrease_around( row - 1, col, row, col );

		if( no_errors )
			_built_solution.emplace_back( coordinates_to_index( row - 1, col ), coordinates_to_index( row, col ) );
		return no_errors;
	}
	if( col + 1 < _width && _grid[row][col + 1] > 0 ) // East
	{
		no_errors = no_errors && fill( row, col );
		no_errors = no_errors && fill( row, col + 1 );
#if defined DEBUG
		std::cout << "E-Filling ([" << row << "][" << col << "]), ([" << row << "][" << col+1 << "])\n";
#endif
		no_errors = no_errors && decrease_around( row, col, row, col + 1 );

		if( no_errors )
			_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row, col + 1 ) );
		return no_errors;
	}
	if( row + 1 < _height && _grid[row + 1][col] > 0 ) // South
	{
		no_errors = no_errors && fill( row, col );
		no_errors = no_errors && fill( row + 1, col );
#if defined DEBUG
		std::cout << "S-Filling ([" << row << "][" << col << "]), ([" << row+1 << "][" << col << "])\n";
#endif
		no_errors = no_errors && decrease_around( row, col, row + 1, col );

		if( no_errors )
			_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row + 1, col ) );
	}
	return no_errors;
}

#if defined V2
bool Solution::force_move( int r1, int c1, int r2, int c2 )
{
#if defined DEBUG
	std::cout << "Force " << r1 << ","<< c1 << " " << r2 << ","<< c2 << "\n";
#endif
	bool no_errors = true;
	no_errors = no_errors && fill( r1, c1 );
	no_errors = no_errors && fill( r2, c2 );
	
	if( no_errors )
		_built_solution.emplace_back( coordinates_to_index( r1, c1 ), coordinates_to_index( r2, c2 ) );
	if( _grid[r1][c1-1] != -1 )
		no_errors = no_errors && decrease( r1, c1 - 1 );
	if( _grid[r2][c2+1] != -1 )
		no_errors = no_errors && decrease( r2, c2 + 1 );
	if( _grid[r1-1][c1] != -1 )
		no_errors = no_errors && decrease( r1 - 1, c1 );
	if( _grid[r2-1][c2] != -1 )
		no_errors = no_errors && decrease( r2 - 1, c2 );

	return no_errors; 
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
#endif

std::vector< std::pair<int,int> > Solution::build()
{
	init_grid();
	
	for( int row = 0 ; row < _height ; ++row )
	{
		for( int col = 0 ; col < _width ; ++col )
		{
#if defined DEBUG
			std::cout << "Looking at _grid[" << row << "]["<< col<< "]\n";
#endif
			if( _grid[row][col] != -1 )
			{
#if defined DEBUG
				std::cout << "Unfilled cell\n";
#endif
				if( !fill( row, col ) )
					return _built_solution;
				
				if( toss_coin() ) //horizontal tuple
				{
#if defined DEBUG
					std::cout << "Horizontal\n";
#endif
					if( !fill( row, col + 1 ) )
						return _built_solution;
					_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row, col + 1 ) );
					
					if( col + 2 < _width && _grid[row][col+2] != -1 )
						if( !decrease( row, col + 2 ) )
							return _built_solution;
					if( row + 1 < _height )
					{
						if( _grid[row+1][col] != -1 )
							if( !decrease( row + 1, col ) )
								return _built_solution;
						if( _grid[row+1][col+1] != -1 )
							if( !decrease( row + 1, col + 1 ) )
								return _built_solution;
					}
				}
				else //vertical tuple
				{
#if defined DEBUG
					std::cout << "Vertical _grid[" << row+1 << "][" << col << "]=" << _grid[row+1][col] << "\n";
#endif
					if( !fill( row + 1, col ) )
						return _built_solution;
					_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row + 1, col ) );

					if( row + 2 < _height && _grid[row+2][col] != -1 )
						if( !decrease( row + 2, col ) )
							return _built_solution;
					if( col + 1 < _width )
					{
						if( _grid[row][col+1] != -1 )
							if( !decrease( row, col + 1 ) )
								return _built_solution;
						if( _grid[row+1][col+1] != -1 )
							if( !decrease( row + 1, col + 1 ) )
								return _built_solution;
					}
					if( col - 1 >= 0 && _grid[row+1][col-1] != -1 )
						if( !decrease( row + 1, col - 1 ) )
							return _built_solution;
#if defined V2
					if( row == _height - 3 && _grid[row+2][col] != -1 && col > 1 && col < _width - 2 )
					{
						if( left_scan_is_even( col ) )
							force_move( row + 2, col, row + 2, col + 1 );
						else
							force_move( row + 2, col - 1, row + 2, col );
					}
#endif
				}
#if defined DEBUG
				for( int a = 0 ; a <= 4 ; ++a )
					std::cout << "_count[" << a << "]=" << _count[a] << "\n";
#endif
				while( _count[1] > 0 )
				{
					if( !greedy_fill( coordinates_to_index( row, col ) ) )
						return _built_solution;
#if defined DEBUG
					for( int a = 0 ; a <= 4 ; ++a )
					 	std::cout << "_count[" << a << "]=" << _count[a] << "\n";
#endif
				}
			}
		}
	}

	_is_valid = true;
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

