#include "floor.h"
#include <cmath>
#include <queue>
#include <stack>
#include <random>
#include <algorithm>
#include <utility>
#include <cassert>


std::random_device rd;
std::mt19937 mt(rd());
/**
 * @brief chooseRandomOp returns a random Op
 * @return Random Op
 */
Op chooseRandomOp()
{
    //TODO: Implement random operator
    //const int num_of_exp = 20000;
    std::uniform_int_distribution<int> distribution(0,1);
    int number = distribution(mt);

    //double expectedProbability = double(1)/double(2);
    //std::cout << p[0]/num_of_exp << "," << p[1]/num_of_exp << std::endl;
    if (number == 0){
        return Op::Horizontal;
    }
    else {
        return Op::Vertical;
    }
    return Op::None;
}

/**
 * @brief switchOp rotates a given Op
 * @param op
 */
void switchOp(Op& op)
{

    // TODO: Implement operator switch
    if (op == Op::Horizontal){
        op = Op::Vertical;
    }else {
        op = Op::Horizontal;
    }
}

/**
 * @brief operator << prints an Op as + or *.
 */
std::ostream& operator<<(std::ostream& os, Op op)
{
    switch(op)
    {
        case Op::None:       return os<<"";
        case Op::Horizontal: return os<<"+";
        case Op::Vertical:   return os<<"*";
        default: return os;
    }
}

/**
 * @brief Floor::setRoot sets the root node.
 * @param root
 */
void Floor::setRoot (Node* root)
{
    this->root = root;
}


/**
 * @brief Floor::Floor constructs an initial slicing tree (cellNodes and innerNodes) for a given set of cells.
 * @param cells
 */
Floor::Floor(const std::vector<Cell>& cells): root(nullptr)
{
    // TODO: Implement initial tree creation here
    for (auto& cell : cells) {
        this->cellNodes.push_back(std::make_unique<Node>(Node(cell)));
    }
    for (unsigned i = 0; i < cells.size() - 1  ; ++i){
        this->innerNodes.push_back(std::make_unique<Node>(Node(chooseRandomOp())));
    }

  Node* current;
  Node* old;
  size_t innerIndex = 0;
  size_t cellIndex = 0;
  std::cout << this->innerNodes.size() << ", " << this->cellNodes.size() << std::endl;
  while (innerIndex < this->innerNodes.size()) {

      current = this->innerNodes[innerIndex].get();
      if (innerIndex!=0){
          current->parent = old;
         }
      else {
          current->parent = nullptr;
      }
      current->left = this->cellNodes[cellIndex].get();
      current->left->parent = current;
      cellIndex++;
      innerIndex++ ;
      std::cout << cellIndex << ", " << innerIndex << std::endl;
      if (innerIndex == this->innerNodes.size()){
          current->right = this->cellNodes[cellIndex].get();
          current->right->parent = current;
          //break;
      } else {
          current->right = this->innerNodes[innerIndex].get();
      }
       // innerIndex++ ;
      //std::cout << current << ", " << current->parent << ", " <<  current->left << ", " << current->right << std::endl;
      old = current;
  }

  this->setRoot(this->innerNodes[0].get());

    //return;
}


/**
 * @brief Floor::calcSize determines width and height of a packed (subtree) floorplan
 * @param node Root node of the (subtree) floorplan
 * @return A tuple of width and height
 */
std::tuple<size_t,size_t> Floor::calcSize(Node* node)
{

    // TODO: Implement size calculation beginning at the given node
    if (node){
    if (node->op == Op::None){
        return std::tuple<size_t,size_t>(node->cell.getXlength(),node->cell.getYlength());
    }
    if (node->op == Op::Vertical){
        return calcVerticalSize(node);
    }
    if (node->op == Op::Horizontal){
        return calcHorizontalSize(node);
    }
    }
    return std::make_tuple(0,0);
}

/**
 * @brief Floor::calcVerticalSize determines the size of a vertical cut (sub)tree.
 * @param node Root node of the (sub)tree (assumed to be a vertical cut node)
 * @return A tuple of width and hight
 */
std::tuple<size_t,size_t> Floor::calcVerticalSize(Node* node)
{
    // TODO: Implement vertical cut size calculation beginning at the given node
    if (node){
        return std::tuple<size_t,size_t>(std::get<0>(calcSize(node->left)) + std::get<0>(calcSize(node->right)),std::max(std::get<1>(calcSize(node->left)), std::get<1>(calcSize(node->right))));
    }
    return std::make_tuple(0,0);
}

/**
 * @brief Floor::calcHorizontalSize determines the size of a horizontal cut (sub)tree
 * @param node Root node of the (sub)tree (assumed to be a horizontal cut node)
 * @return A tuple of width and height
 */
std::tuple<size_t,size_t> Floor::calcHorizontalSize(Node *node)
{
    // TODO: Implement horizontal cut size calculation beginning at the given node
    if (node) {
    return std::tuple<size_t,size_t>(std::max(std::get<0>(calcSize(node->left)),std::get<0>(calcSize(node->right))),std::get<1>(calcSize(node->left)) + std::get<1>(calcSize(node->right)));
    }
    return std::make_tuple(0,0);
}

/**
 * @brief Floor::pack resizes this->plan and fills it with the cell names at the occupied positions
 * @param coordinates
 */
void Floor::pack()
{
    this->plan.clear();
    auto [maxX, maxY] = this->calcSize(this->getRoot());
    this->plan.resize(maxY, std::vector<std::string>(maxX,"0"));
    this->addCells(Coordinates(0,0), this->getRoot());
}

/**
 * @brief Floor::shiftRight performs a right shift rotation.
 * @param node
 */
void Floor::shiftRight(Node *node)
{
    assert(node->left);
    if(node->left->op == Op::None)
        return;
    auto c = node->left;
    if(node->parent)
    {
        if(node->parent->left == node)
            node->parent->left = c;
        if(node->parent->right == node)
            node->parent->right = c;
    }
    if(getRoot() == node)
        setRoot(c);
    c->right->parent = node;
    c->parent    = node->parent;
    node->left   = c->right;
    c->right     = node;
    node->parent = c;
}

/**
 * @brief Floor::shiftLeft erforms a left shift rotation.
 * @param node
 */
void Floor::shiftLeft(Node *node)
{
    assert(node->right);
    if(node->right->op == Op::None)
        return;
    auto c = node->right;
    if(node->parent)
    {
        if(node->parent->left == node)
            node->parent->left = c;
        if(node->parent->right == node)
            node->parent->right = c;
    }
    if(getRoot() == node)
        setRoot(c);
    c->left->parent = node;
    c->parent    = node->parent;
    node->right  = c->left;
    c->left      = node;
    node->parent = c;
}

/**
 * @brief Floor::modify applies an arbitrary modification on the slicing tree.
 */
void Floor::modify()
{
    // TODO: Implement method
    std::uniform_int_distribution<int> distribution(0,1);
    int number = distribution(mt);
    if(number == 0){// inner node
        std::uniform_int_distribution<int> distributionForPosition(0,this->innerNodes.size()-1);
        int position = distributionForPosition(mt);
        std::uniform_int_distribution<int> distributionForOperation(0,2);
        int operation = distributionForOperation(mt);
        if (operation == 0){
            switchOp(this->innerNodes[position]->op);
        }
        if (operation == 1){
            Node* holder = this->innerNodes[position]->left;
            this->innerNodes[position]->left = this->innerNodes[position]->right;
            this->innerNodes[position]->right = holder;
        }
        if (operation == 2){
            std::uniform_int_distribution<int> distributionForShift(0,1);
            int shift = distributionForShift(mt);
            Node* node = this->innerNodes[position].get();
            if (shift == 0){
                shiftLeft(node);
            }
            else {
               shiftRight(node);
            }
        }
       // this->innerNodes[position];
    } else {
        std::uniform_int_distribution<int> distributionForPosition(0,this->cellNodes.size()-1);
        int position = distributionForPosition(mt);
        this->cellNodes[position]->cell.rotate();
    }
   // return;
}

/**
 * @brief Floor::optimize is a simple monotonic optimizer.
 * The flooplan is modified randomly t times.
 * The smalles floorplan result is returned in this->plan.
 * @param t Number of iterations
 */
void Floor::optimize(size_t t)
{
    // TODO: Implement method
    //this->pack();
    size_t currentArea;
    currentArea = this->calcArea(this->plan);
    for (size_t i = 0; i < t ; ++i ){
        //currentArea = this->calcArea(this->plan);
        this->modify();
        size_t area = std::get<0>(this->calcSize(this->getRoot()))*std::get<1>(this->calcSize(this->getRoot()));
        if (area < currentArea) {
            //this->pack();
            this->pack();
            currentArea = area;

        }

    }
    //return;
}

/**
 * @brief Floor::addCells fills the cells of a given subtree into the this->plan member.
 * @param coordinates
 * @param node
 */
void Floor::addCells(Coordinates coordinates, Node* node)
{
    if (!node)
        return;
    switch(node->op)
    {
        case Op::None:
        {
            size_t x = coordinates.x + node->cell.getXlength();
            size_t y = coordinates.y + node->cell.getYlength();

            for (size_t jy = coordinates.y; jy < y; ++jy)
                for (size_t ix = coordinates.x; ix < x; ++ix)
                    this->plan[jy][ix] = node->cell.getName();
            break;
        }
        case Op::Horizontal:
            this->addCells(coordinates,node->left);
            this->addCells(Coordinates(coordinates.x,coordinates.y+std::get<1>(this->calcSize(node->left))),node->right);
            break;
        case Op::Vertical:
            this->addCells(coordinates,node->left);
            this->addCells(Coordinates(coordinates.x+std::get<0>(this->calcSize(node->left)),coordinates.y),node->right);
            break;
    }
    return;
}

/**
 * @brief Floor::printPostOrder prints the slicing tree in post order.
 * @param node
 */
void Floor::printPostOrder(Node *node)
{
    if(!node)
        return;
    printPostOrder(node->left);
    printPostOrder(node->right);
    std::cout << node->cell.getName() << node->op << " " << std::flush;
}

/**
 * @brief Floor::calcArea returns the area of a given plan.
 * @param plan
 */
size_t Floor::calcArea(const std::vector <std::vector<std::string>>& plan)
{
    if(!plan.size())
        return 0;
    return plan.size() * plan[plan.size()-1].size();
}

/**
 * @brief operator << prints an ASCII art of a Floor::plan
 */
std::ostream& operator<<(std::ostream& os, const Floor& floor)
{
    for (const auto& line: floor.plan)
    {
        for (const auto& cell: line)
            os << cell << "  ";
        os << "\n";
    }
    return os;
}

