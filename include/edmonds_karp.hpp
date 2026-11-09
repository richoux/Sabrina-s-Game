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

		// Statistics
		unsigned long _number_iterations;
		unsigned long _memory_usage;
		double _runtime;

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

		inline unsigned int get_number_iterations() const { return _number_iterations; }
		inline unsigned int get_memory_usage() const { return _memory_usage; }
		inline double get_runtime() const { return _runtime; }
	};
}
