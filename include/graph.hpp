#pragma once

#include <vector>
#include <deque>
#include <utility>

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
		inline std::deque<int> get_cells_from( int cell ) { return _edges[cell]; }
		inline int get_source() { return _source; }
		inline int get_sink() { return _sink; }
		
		std::vector<std::pair<int,int>> edges_from_white_cells() const;
		
		void display() const;
	};
}
