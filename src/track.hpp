#include <vector>

#include "simulation.hpp"

namespace track{
    class Track{
	public:
	
	std::vector<simulation::Position> vertices;

	Track(simulation::Position origin);	

        std::vector<simulation::Position> get_vertices();
	
    };
}
