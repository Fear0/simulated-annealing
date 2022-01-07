
#include "floor.h"
#include <chrono>
//#include "floor.cpp"

int main(){
    std::vector<Cell>cells;
    cells.emplace_back(3,4,"A");
    cells.emplace_back(4,2,"B");
    cells.emplace_back(2,3,"C");
    cells.emplace_back(2,7,"D");
    cells.emplace_back(3,1,"E");
    cells.emplace_back(2,5,"F");
    cells.emplace_back(1,1,"G");

    Floor floor(cells);
    std::string s;
    floor.printPostOrder(floor.getRoot());
    std::cout << std::endl;
    std::cout << "Width: " << std::get<0>(floor.calcSize(floor.getRoot())) << " "
              << "Height: " << std::get<1>(floor.calcSize(floor.getRoot())) << std::endl;
    std::cout << std::endl;
    floor.pack();
    std::cout << "Floorarea: " << floor.calcArea(floor.plan) << std::endl;
    std::cout << floor << std::endl;
    std::cout << std::endl;

    std::cout << "Monotonic optimization: " << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    floor.optimize(1000000);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Floorarea: " << floor.calcArea(floor.plan) << std::endl;
    std::cout << floor << std::endl;
    std::cout << "Monotonic optomization lasted "<< duration.count() << "ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();

    Floor floor1(cells);
    floor1.pack();
    std::cout << std::endl;
    std::cout << "Simulated Annealing: " << std::endl;
    auto result = floor1.siumaltedAnnealing();
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Floor Area: " << std::get<1>(result) << std::endl;
    floor.printFloor(std::get<0>(result));
    std::cout << std::endl;
    std::cout << "Simulated Annealing optomization lasted "<< duration.count() << "ms" << std::endl;

}

