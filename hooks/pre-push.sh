#!/bin/sh

# if build directory does not exist, create it
if [ ! -d "./build" ]; then
  mkdir build
fi

# build the project with cmake
cd ./build
cmake -DBUILD_TESTS=OFF -DBUILD_DOC=ON ..
cmake --build .

# deploy docs to vercel
bun x vercel deploy --prod doxygen/html
