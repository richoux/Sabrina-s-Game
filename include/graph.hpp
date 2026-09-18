#pragma once

#include <vector>
#include <deque>
#include <utility>
#include <map>

namespace sabrinasgame
{
	class Graph {
		int _width;
		int _height;
		int _nb_cells;
		int _source;
		int _sink;
		
		std::vector<std::deque<int>> _edges;
		std::vector<int> _white_cells; // connected to the source
		std::vector<int> _black_cells; // connected to the sink
				
		void add_edges_around( int cell );
	
	public:

		Graph( int widht, int height, bool empty = false );

		inline void add_edge( int cell1, int cell2 ) { _edges[cell1].push_back( cell2 ); }
		inline void remove_edge( int cell1, int cell2 ) { std::erase(	_edges[cell1], cell2 ); }
		inline std::deque<int> get_cells_from( int cell ) const { return _edges[cell]; }
		inline int get_source() const { return _source; }
		inline int get_sink() const { return _sink; }
		inline std::vector<int> get_white_cells() const { return _white_cells; }
		inline std::vector<int> get_black_cells() const { return _black_cells; }
		
		std::vector<std::pair<int,int>> edges_from_white_cells() const;
		std::deque<int> get_cells_from_excluding( int cell, const std::vector<int>& to_exclude ) const;
		std::deque<int> get_filtered_cells_from( int cell,
		                                         const std::vector<int>& to_exclude,
		                                         const std::map<int,int>& visits ) const;
			
		void display() const;
	};
}
