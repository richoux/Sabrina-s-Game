#include <ctime>
#include <cassert>
#include <array>

#include "solution.hpp"
#include "utils.hpp"

using sabrinasgame::Solution;

enum Direction{UP, RIGHT, BOTTOM, LEFT};

bool Solution::fill( int row, int col )
{
#if defined DEBUG
	std::cout << "Filling (" << row << "," << col << ") " << _grid[row][col] << "\n";
	assert(_grid[row][col] != -1);
#endif
	if( _grid[row][col] == -1 )
		return false;
	else
	{
		if( _grid[row][col] == 2 )
			std::erase( _list_count2, coordinates_to_index( row, col ) );
		else
			if( _grid[row][col] == 1 )
				std::erase( _list_count1, coordinates_to_index( row, col ) );
	}
	
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
	else
	{
		if( _grid[row][col] == 3 )
		{
			// Only add corner cells
			if( !( is_free( row+1, col) && is_free( row-1, col) ) && !( is_free( row, col-1) && is_free( row, col+1) ) )
			{
				_list_count2.push_back( coordinates_to_index( row, col ) );
			}
		}
		else
		{
			if( _grid[row][col] == 2 )
			{
				std::erase( _list_count2, coordinates_to_index( row, col ) );
				_list_count1.push_back( coordinates_to_index( row, col ) );
			}
			else
				if( _grid[row][col] == 1 )
					std::erase( _list_count1, coordinates_to_index( row, col ) );
		}
	}

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

#if defined DEBUG
	std::cout << "End decrease_around\n";
#endif
	return true;
}

std::vector< std::pair<int,int> > Solution::build()
{
	init_grid();

	int index;
	int row, col;
	std::array<Direction,4> directions{UP, RIGHT, BOTTOM, LEFT};	
	
	while( !_list_count1.empty() || !_list_count2.empty() )
	{
		if( _count[1] > 0 )
		{
			auto element = _rng.choose( _list_count1 );
			index = *element;
			_list_count1.erase( element );
		}
		else
		{
			auto element = _rng.choose( _list_count2 );
			index = *element;
			_list_count2.erase( element );
		}

		auto coords = index_to_coordinates( index );
		row = coords.first;
		col = coords.second;
	
		if( !fill( row, col ) )
			return _built_solution;

		_rng.shuffle( directions );

		bool direction_found = false;
		for( auto direction: directions )
		{
			if( direction_found )
				break; // break loop

#if defined DEBUG
			std::cout << "Direction: " << direction << "\n";
#endif

			switch( direction )
			{
			case UP:
				if( row - 1 >= 0 && _grid[row-1][col] != -1 )
				{
#if defined DEBUG
					std::cout << "Decreasing UP\n";
#endif			
					if( !fill( row - 1, col ) )
						return _built_solution;
					
					_built_solution.emplace_back( coordinates_to_index( row - 1, col ), coordinates_to_index( row, col ) );

					if( !decrease_around( row - 1, col, row, col ) )
						return _built_solution;

					direction_found = true;
				}
				break;
			case RIGHT:
				if( col + 1 < _width && _grid[row][col+1] != -1 )
				{
#if defined DEBUG
					std::cout << "Decreasing RIGHT\n";
#endif			
					if( !fill( row, col + 1 ) )
						return _built_solution;
					
					_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row, col + 1 ) );

					if( !decrease_around( row, col, row, col + 1 ) )
						return _built_solution;

					direction_found = true;						
				}
				break;
			case BOTTOM:
				if( row + 1 < _height && _grid[row+1][col] != -1 )
				{
#if defined DEBUG
					std::cout << "Decreasing BOTTOM\n";
#endif			
					if( !fill( row + 1, col ) )
						return _built_solution;
					
					_built_solution.emplace_back( coordinates_to_index( row, col ), coordinates_to_index( row + 1, col ) );

					if( !decrease_around( row, col, row + 1, col ) )
						return _built_solution;

					direction_found = true;						
				}
				break;
			case LEFT:
				if( col - 1 >= 0 && _grid[row][col-1] != -1 )
				{
#if defined DEBUG
					std::cout << "Decreasing LEFT\n";
#endif			
					if( !fill( row, col - 1 ) )
						return _built_solution;
					
					_built_solution.emplace_back( coordinates_to_index( row, col - 1 ), coordinates_to_index( row, col ) );

					if( !decrease_around( row, col - 1, row, col ) )
						return _built_solution;

					direction_found = true;						
				}
				break;
			}
		}
	}

	_is_valid = true;
	return _built_solution;
}
