#pragma once

#include "graph.hpp"
#include "../thirdparty/randutils.hpp"

namespace sabrinasgame
{
	class EdmondsKarp
	{
		int _width;
		int _height;
		
		Graph _residuals;
		Graph _flows;

		std::vector<std::deque<int>> _residual_outgoing_edges;
		std::vector<std::deque<int>> _residual_incoming_edges;
		
		randutils::mt19937_rng _rng;

		void recursive_remove_outgoing_edges( int node, int parent );

		void filtering_neighbors_of( int node,
		                             bool is_white,
		                             const std::vector<int>& path_to_exclude,
		                             const std::map<int,int>& visits );

		struct Nodepath
		{
			int node;
			Nodepath *parent;

			Nodepath( int n )
				: node(n),
				  parent(nullptr)
			{ }

			Nodepath( int n, Nodepath *p )
				: node(n),
				  parent(p)
			{ }
		};
		
		std::vector<int> random_bfs( const std::vector<int>& path );
		
	public:
		EdmondsKarp( int width, int height );
		Graph solve();
		void print() const;
	};
}
