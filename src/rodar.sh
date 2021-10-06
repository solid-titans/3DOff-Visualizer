g++ $(pkg-config --cflags --libs Qt5Widgets) -fPIC -o main main.cpp
./main