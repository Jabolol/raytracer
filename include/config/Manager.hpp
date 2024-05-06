#include <libconfig.hh>

#ifndef __CFG_MANAGER_HPP__
    #define __CFG_MANAGER_HPP__

namespace RayTracer::Config
{
    class Manager {
      private:
        libconfig::Config config;

      public:
        void parse(std::string_view path);
    };
} // namespace RayTracer::Config

#endif /* __CFG_MANAGER_HPP__ */
