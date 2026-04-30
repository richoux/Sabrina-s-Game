#pragma once

#include <vector>
#include <utility> //std::pair
#include <random>
#include <iostream>

namespace sabrinasgame
{
	class Solution
	{
		int _width;
		int _height;
		std::vector< std::vector<int> > _grid;
		int _count[5];
		std::vector< std::pair<int,int> > _built_solution;
		bool _is_valid;
		
		std::random_device _rd;
    std::mt19937 _gen;
    std::uniform_int_distribution<> _distrib;

		bool toss_coin();		
		void init_grid();
		bool fill( int row, int col );
		bool decrease( int row, int col );
		bool greedy_fill( int index );
		bool decrease_around( int row1, int col1, int row2, int col2 );
		
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
