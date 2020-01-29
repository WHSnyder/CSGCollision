clang++ -Iinclude -Isrc $(pkg-config --cflags --libs opencv4)  -std=c++17 ./src/Main.cpp ./src/CSG.cpp ./src/Scene.cpp ./src/primitives/Tri.cpp ./src/primitives/Plane.cpp ./src/primitives/Sphere.cpp -o ./bin/out