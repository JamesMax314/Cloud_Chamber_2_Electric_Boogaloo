#include <vector>
#include <random>
#include <cmath>

#define GLM_FORCE_PURE
#include <glm/glm.hpp>

#include "simulation.hpp"

namespace track{
    class Track{
	public:
	
	std::vector<simulation::Position> vertices;

	//Random number engine
	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform_dist;

	Track(simulation::Position origin);	

        std::vector<simulation::Position> get_vertices();
	glm::vec3 gen_rand_direction();

    };
}
