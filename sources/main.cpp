#include "config/Manager.hpp"

int main(int argc, char **argv)
{
    Raytracer::Config::Manager manager;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
        return 84;
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
