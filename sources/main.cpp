#include "config/Manager.hpp"

int main(int argc, char **argv)
{
    Raytracer::Config::Manager manager;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
        return 84;
    }

    std::string flag = "--fast";
    bool fast = std::find(argv, argv + argc, flag) != argv + argc;
    auto path = std::find_if(argv + 1, argv + argc, [](const char *arg) {
        return arg[0] != '-';
    });

    try {
        manager.parse(*path);
        manager.bootstrap();
        manager.render(fast);
    } catch (const Raytracer::Exceptions::Base &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}
