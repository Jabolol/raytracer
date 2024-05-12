#include "config/Manager.hpp"

int main(int argc, char **argv)
{
    Raytracer::Config::Manager manager;

    std::string usage = "Usage: " + std::string(argv[0])
        + " [--fast] --config <config file>\n";

    if (argc < 2) {
        std::cerr << usage;
        return 84;
    }

    if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
        std::cout << usage;
        return 0;
    }

    bool fast = false;
    std::string path = "/dev/null";

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--config") {
            if (i + 1 < argc) {
                path = argv[i + 1];
            }
        } else if (std::string(argv[i]) == "--fast") {
            fast = true;
        }
    }

    bool success = manager.parse(path);

    if (!success) {
        return 84;
    }

    manager.bootstrap();
    manager.render(fast);

    return 0;
}
