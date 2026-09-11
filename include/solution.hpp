#pragma once

#include <vector>
#include <deque>
#include <utility> //std::pair
#include <iostream>

#include "../thirdparty/randutils.hpp"

namespace sabrinasgame
{
	class Solution
	{
		int _width;
		int _height;
		std::vector< std::vector<int> > _grid;
		int _count[5];
		std::deque<int> _list_count1;
		std::deque<int> _list_count2;

		std::vector< std::pair<int,int> > _built_solution;
		bool _is_valid;
		
		randutils::mt19937_rng _rng;

		bool toss_coin();		
		void init_grid();
		bool fill( int row, int col );
		bool decrease( int row, int col );
		bool greedy_fill( int index );
		bool decrease_around( int row1, int col1, int row2, int col2 );
		bool left_scan_is_even( int col ) const;
		bool force_move( int row1, int col1, int row2, int col2 );

		inline bool is_free( int row, int col ) { return row >= 0 && row < _height && col >=0 && col < _width && _grid[row][col] > 0; }

	public:
		Solution( int width, int height );

		std::vector< std::pair<int,int> > build();
		void print() const;
		inline bool is_valid() const { return _is_valid; }

		
		std::pair<int,int> index_to_coordinates( int index ) const;
		int coordinates_to_index( int, int ) const;
		int coordinates_to_index( const std::pair<int,int>& ) const;
	};
}
