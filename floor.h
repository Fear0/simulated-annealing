#ifndef FLOORPLANNING_FLOOR_H
#define FLOORPLANNING_FLOOR_H

#include <iostream>
#include "cell.h"
#include <vector>
#include <map>
#include <memory>

//std::vector<int> modificationsHolder;
/**
 * @brief An Op determins what kind of cut an inner Node represents.
 */
enum class Op
{
    None,
    Horizontal,
    Vertical
};

/**
 * @brief Nodes build the slicing tree of a floorplan (Floor).
 * Nodes are either a cut operation (innerNode) or a cell (cellNode).
 * Cell nodes are the leafs of the tree and they are indicated by the Op Op::None.
 * @see Floor
 * @see Op
 * @see Cell
 */
struct Node
{
public:
    Node(const Cell &cell) : parent(nullptr), left(nullptr), right(nullptr), cell(cell), op(Op::None) {}
    Node(Op op) : parent(nullptr), left(nullptr), right(nullptr), cell(Cell(0, 0, "")), op(op) {}
    Node() = delete;
    Node *parent;
    Node *left;
    Node *right;
    Cell cell;
    Op op;
};

/**
 * @brief A Floor contains a slicing tree representation of a floorplan.
 */
class Floor
{
private:
    Node *root = nullptr;
    void setRoot(Node *root);
    void shiftLeft(Node *node);
    void shiftRight(Node *node);
    void addCells(Coordinates coordinates, Node *node);
    //void swapOperator(Node* node);
    std::tuple<size_t, size_t> calcHorizontalSize(Node *node);
    std::tuple<size_t, size_t> calcVerticalSize(Node *node);

public:
    std::vector<std::shared_ptr<Node>> cellNodes;
    std::vector<std::shared_ptr<Node>> innerNodes;
    Floor(const std::vector<Cell> &cells);
    Floor() : root(nullptr){};
    std::vector<std::vector<std::string>> plan;
    Node *getRoot() { return this->root; };
    void pack();
    void modify();
    void reverse();
    void optimize(size_t t);
    std::tuple<std::vector<std::vector<std::string>>, size_t> siumaltedAnnealing();
    size_t calcArea(const std::vector<std::vector<std::string>> &plan);
    std::tuple<size_t, size_t> calcSize(Node *node);
    void printPostOrder(Node *node);
    double initialTemperatue();
    void printFloor(const std::vector<std::vector<std::string>> &floor);
    void postOrderString(Node *node, std::string &s);
};

std::ostream &operator<<(std::ostream &os, const Floor &);

#endif //FLOORPLANNING_FLOOR_H
