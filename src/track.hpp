#include <vector>
#include <random>

#include "simulation.hpp"

namespace track{
    class Track{
	public:
	
	std::vector<simulation::Position> vertices;

	Track(simulation::Position origin);	

        std::vector<simulation::Position> get_vertices();

	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;
    };
}
