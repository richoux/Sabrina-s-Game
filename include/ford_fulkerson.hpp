#pragma once

#include "graph.hpp"
#include "../thirdparty/randutils.hpp"

namespace sabrinasgame
{
	class FordFulkerson
	{
		int _width;
		int _height;
		
		Graph _residuals;
		Graph _flows;

		randutils::mt19937_rng _rng;
		
	public:
		FordFulkerson( int width, int height );
		Graph solve();
		void print() const;
	};
}
