#include "config/Manager.hpp"

int main(int argc, char **argv)
{
    Raytracer::Config::Manager manager;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
        return 84;
    }

    try {
        manager.parse(argv[1]);
        manager.bootstrap();
        manager.render();
    } catch (const Raytracer::Exceptions::Base &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}
