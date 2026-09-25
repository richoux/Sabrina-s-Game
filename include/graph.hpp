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
		int _nb_nodes;
		int _source;
		int _sink;
		
		std::vector<std::deque<int>> _outgoing_edges;
		std::vector<std::deque<int>> _incoming_edges;
		std::vector<int> _white_nodes; // connected to the source
		std::vector<int> _black_nodes; // connected to the sink
				
		void add_edges_around( int node );
	
	public:

		Graph( int widht, int height, bool empty = false );

		inline int get_nb_nodes() const { return _nb_nodes; }
		inline int get_source() const { return _source; }
		inline int get_sink() const { return _sink; }

		inline std::vector<std::deque<int>> get_outgoing_edges() const { return _outgoing_edges; }
		inline std::vector<std::deque<int>> get_incoming_edges() const { return _incoming_edges; }
		inline std::deque<int> get_neighbors_of( int node ) const { return _outgoing_edges[node]; }
		inline std::deque<int> get_parents_of( int node ) const { return _incoming_edges[node]; }
		inline std::vector<int> get_white_nodes() const { return _white_nodes; }
		inline std::vector<int> get_black_nodes() const { return _black_nodes; }
		
		void add_edge( int node1, int node2 );
		void remove_edge( int node1, int node2 );
		std::vector<std::pair<int,int>> edges_from_white_nodes() const;
			
		void display() const;
	};
}
