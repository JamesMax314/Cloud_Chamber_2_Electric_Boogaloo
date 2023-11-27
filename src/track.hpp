#include <vector>
#include <random>
#include <cmath>

#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"

#include "simulation.hpp"
#include "utils.hpp"

namespace track{

    class Track{

	public:
	
		std::vector<simulation::Position> vertices;

		Track(simulation::Position origin);	

        std::vector<simulation::Position> get_vertices();

    };

	class Alpha_Track: public Track{

		public:

			Alpha_Track(simulation::Position origin, double energy);

	};
}
