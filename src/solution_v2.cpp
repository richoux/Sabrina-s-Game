#include <ctime>
#include <cassert>

#include "solution.hpp"
#include "utils.hpp"

using sabrinasgame::Solution;

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
	bool trigger_forced_move = false;
	
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
		if( is_vertical && row2 == _height - 2 && col2 > 1 && col2 < _width - 2 )
			trigger_forced_move = true;
#if defined DEBUG
		std::cout << "S\n";
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

	if( trigger_forced_move )
	{
		if( left_scan_is_even( col2 ) )
			force_move( row2 + 1, col2, row2 + 1, col2 + 1 );
		else
			force_move( row2 + 1, col2 - 1, row2 + 1, col2 );
	}
	
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
					if( row == _height - 3 && _grid[row+2][col] != -1 && col > 1 && col < _width - 2 )
					{
						if( left_scan_is_even( col ) )
							force_move( row + 2, col, row + 2, col + 1 );
						else
							force_move( row + 2, col - 1, row + 2, col );
					}
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
