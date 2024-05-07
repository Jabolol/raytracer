#include "arguments/Materials.hpp"
#include "arguments/Shapes.hpp"
#include "config/Factory.hpp"
#include "config/Manager.hpp"
#include "core/Camera.hpp"
#include "core/Scene.hpp"

int main(int argc, char **argv)
{
    Raytracer::Config::Manager manager;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
        return 1;
    }

    if (!manager.parse(argv[1])) {
        return 1;
    }

    return 0;
}
