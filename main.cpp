
#include "floor.h"

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
    floor.printPostOrder(floor.getRoot());
    Node* root = floor.getRoot();
    //floor.shiftLeft(root);
    std::cout << std::endl;
    floor.printPostOrder(floor.getRoot());
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Width: " << std::get<0>(floor.calcSize(floor.getRoot())) << " "
              << "Height: " << std::get<1>(floor.calcSize(floor.getRoot())) << std::endl;
    std::cout << std::endl;
    floor.pack();
    std::cout << "Floorarea: " << floor.calcArea(floor.plan) << std::endl;
    std::cout << floor <<std::endl;
    floor.optimize(1000000);
    std::cout << std::endl;
    std::cout << "Floorarea: " << floor.calcArea(floor.plan) << std::endl;
    std::cout << floor <<std::endl;
    std::string s;
    //std::cin >> s;
}

