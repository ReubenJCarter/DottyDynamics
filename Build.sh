mkdir -p ./build/obj

#build the test
g++ -O3 -march=native -fopt-info -o ./build/SOAAOSSpeedTest ./test/SOAAOSSpeedTest.cpp
g++ -O3 -march=native -o ./build/WorldTest ./test/WorldTest.cpp