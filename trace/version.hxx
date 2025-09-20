#ifndef VERSION_HXX
#define VERSION_HXX

#include "config.hpp"
#include <boost/version.hpp>

namespace behin::version {
    void print_boost_version() {
        std::cout << "Boost version: "
                  << BOOST_VERSION / 100000     << "."  // major version
                  << BOOST_VERSION / 100 % 1000 << "."  // minor version
                  << BOOST_VERSION % 100                // patch level
                  << std::endl;
    }

    void print_cmake_version() {
        std::cout << "project name: " <<
            cmake::version::project_name << '\n' <<
            "project version: " <<
            cmake::version::project_version << std::endl;
    }

    void print_version() {
        print_cmake_version();
        print_boost_version();
    }
}

#endif /* VERSION_HXX */
