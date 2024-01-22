//
// Created by layan on 5/3/2023.
//


#ifndef HW1WETDS_AVLTREE_H
#define HW1WETDS_AVLTREE_H

#include <memory>
#include <iostream>
#include <math.h>
#define POS_FACTOR 2
#define NEG_FACTOR -2

/*************************** Class & functions DECLARATIONS *************************/

template <class T, class S> class Node;

template <class T, class S>
class AVLTree
{
    std::shared_ptr<Node<T,S>> root;

    std::shared_ptr<Node<T,S>> biggest_node;

    int getTreeSizeAux(std::shared_ptr<Node<T,S>> node);

    void setBiggest(std::shared_ptr<Node<T,S>> node);

public:
    AVLTree() : root(nullptr), biggest_node(nullptr) {};

    std::shared_ptr<Node<T,S>> get_second_biggest();

    void destroyTreeAux(std::shared_ptr<Node<T,S>> subtree_root);

    //~AVLTree() = default;

    ~AVLTree();

    AVLTree(const AVLTree& tree_to_copy) = default;

    AVLTree& operator=(const AVLTree& tree) = default;

    std::shared_ptr<Node<T,S>> findNode(T Key) const;  //- DONE

    std::shared_ptr<Node<T,S>> getRoot() const; //DONE

    bool insertNode(const T& key, const S& value);  //lul - DONE

    bool removeNode(const T& key);  //dim

    void rotateLeft(std::shared_ptr<Node<T,S>> subtree_root); //lul - DONE

    void rotateRight(std::shared_ptr<Node<T,S>> subtree_root); //lul  - DONE

    void rotateLL(std::shared_ptr<Node<T,S>> subtree_root); //dim - DONE

    void rotateRR(std::shared_ptr<Node<T,S>> subtree_root); //dim - DONE

    void rotateLR(std::shared_ptr<Node<T,S>> subtree_root); //dim - DONE

    void rotateRL(std::shared_ptr<Node<T,S>> subtree_root); //dim - DONE

    void balance(std::shared_ptr<Node<T,S>> subtree_root); //lul  - DONE

    template <class DoSomething, class Param1, class Param2, class Param3> //dim - DONE
    void inOrder(std::shared_ptr<Node<T,S>> subtree_root, DoSomething doSomething, Param1 param1 = 0,
                 Param2 param2 = 0, Param3 param3 = 0);

    template <class DoSomething, class Param1, class Param2, class Param3> //dim - DONE
    void preOrder(std::shared_ptr<Node<T,S>> subtree_root, DoSomething doSomething, Param1 param1=0,
                 Param2 param2=0, Param3 param3=0);

    template <class DoSomething, class Param1, class Param2, class Param3> //dim - DONE
    void postOrder(std::shared_ptr<Node<T,S>> subtree_root, DoSomething doSomething, Param1 param1=0,
                 Param2 param2=0, Param3 param3=0);

    std::shared_ptr<Node<T,S>> findBiggest(); // lul - DONE

    /* for each node that is in the search path of subtree_root: this function updates its height
     * (incerements/ decrements it by 1) and its BF (using the updated heights)
     * */
    void updateAncestorHeightsAndBF(std::shared_ptr<Node<T,S>> subtree_root); // lul - DONE

    /* this function changes ptr to point to the first unblanaced node that is in ptr's search path
     * will use this function for balancing and rotations.
     * might be bugged - will check later
     * */
    std::shared_ptr<Node<T,S>> goToFirstUnbalanced(std::shared_ptr<Node<T,S>> ptr); // lul - DONE

    //void deleteNode(T key);

    bool hasOneSon(std::shared_ptr<Node<T,S>> node);

    std::shared_ptr<Node<T,S>> getNextInorder(std::shared_ptr <Node<T, S>> node);

    //this function checks if this node is a left son or a right son
    //true means right, false means left
    //assumes that node isn't the root. (that parent!=null)
    bool isRightSon(std::shared_ptr<Node<T,S>> node);

    //void buildEmptyTree(int h,std::shared_ptr<Node<T,S>> new_sub_root);

    int getTreeSize();

    /* move_to_array:
     * param1 is the array we want to insert into
     * param2 is the index
     * node contains the value we want to insert inside the array param1
     * */
    /*template <class Param1>
    void move_to_array(Param1 arr);*/

    std::shared_ptr<Node<T,S>> getMax();

};

template <class T, class S>
class Node
{
    T key;

    S value;

    std::shared_ptr<Node<T,S>> left_son;

    std::shared_ptr<Node<T,S>> right_son;

    std::shared_ptr<Node<T,S>> parent;

    int bf;

    int height;

public:
    /* Node: Constructor
     * creates a new node with a key and a value, assigns left and right son and parent to null
     * @param new_key - the new node's key
     * @param new_value - the new node's value
     * */
    Node(const T& new_key, const S& new_value) : key(new_key), value(new_value), left_son(nullptr),
        right_son(nullptr), parent(nullptr), bf(0), height(0) {};

    Node() = default;

    ~Node() = default;

    Node(Node<T,S> &node) = default;

    Node& operator=(const Node& node) = default;

    T& getKey();

    S& getValue();

    int getHeight();

    int getBF();

    std::shared_ptr<Node<T,S>> getLeftSon();

    std::shared_ptr<Node<T,S>> getRightSon();

    std::shared_ptr<Node<T,S>> getParent();

    void setKey(T& new_key);

    void setValue(S& new_value);

    void setHeight(int new_height);

    void setBF(int new_bf);

    void setRightSon(std::shared_ptr<Node<T,S>> new_right_son);

    void setLeftSon(std::shared_ptr<Node<T,S>> new_left_son);

    void setParent(std::shared_ptr<Node<T,S>> new_parent);

    int calcHeight();

    int calcBalanceFactor();

    bool isLeaf();

    bool hasTwoSons();

    void detach();

};

/**************************** Node Class Implementation *******************************/

template<class T,class S>
T& Node<T,S>::getKey()
{
    return key;
}

template<class T,class S>
S& Node<T,S>::getValue()
{
    return value;
}

template<class T,class S>
int Node<T,S>::getHeight()
{
    return height;
}

template<class T,class S>
int Node<T,S>::getBF()
{
    return bf;
}

template<class T,class S>
std::shared_ptr<Node<T,S>> Node<T,S>::getLeftSon()
{
    return left_son;
}


template<class T,class S>
std::shared_ptr<Node<T,S>> Node<T,S>::getRightSon()
{
    return right_son;
}


template<class T,class S>
std::shared_ptr<Node<T,S>> Node<T,S>::getParent(){
    return parent;
}

template<class T,class S>
void Node<T,S>::setKey(T& new_key)
{
    key = new_key;
}

template<class T,class S>
void Node<T,S>::setValue(S& new_value)
{
    value = new_value;
}

template<class T,class S>
void Node<T,S>::setHeight(int new_height)
{
    height = new_height;
}

template<class T,class S>
void Node<T,S>::setBF(int new_bf)
{
    bf = new_bf;
}

template<class T,class S>
void Node<T,S>::setRightSon(std::shared_ptr<Node<T,S>> new_right_son)
{
    right_son = new_right_son;
}

template<class T,class S>
void Node<T,S>::setLeftSon(std::shared_ptr<Node<T,S>> new_left_son)
{
    left_son = new_left_son;
}

template<class T,class S>
void Node<T,S>::setParent(std::shared_ptr<Node<T,S>> new_parent)
{
    parent = new_parent;
}

template<class T,class S>
int Node<T,S>::calcHeight()
{
    int right_son_height = (right_son == nullptr) ? -1 : right_son->getHeight();
    int left_son_height = (left_son == nullptr) ? -1 : left_son->getHeight();
    return (right_son_height > left_son_height) ? right_son_height + 1 : left_son_height + 1; //changes layan +1
}

template<class T,class S>
int Node<T,S>::calcBalanceFactor()
{
    int left_son_height = (left_son== nullptr)? -1 : left_son->getHeight();
    int right_son_height = (right_son == nullptr) ? -1 : right_son->getHeight();
    int balance_factor = left_son_height - right_son_height;
    return balance_factor;
}

template<class T,class S>
bool Node<T,S>::isLeaf()
{
    return ((left_son == nullptr) && (right_son == nullptr));
}

template<class T,class S>
bool Node<T,S>::hasTwoSons()
{
    if(right_son && left_son) return true;
    return false;
}

template<class T,class S>
void Node<T,S>::detach()
{
    parent = nullptr;
    right_son = nullptr;
    left_son = nullptr;
}

/**************************** AVLTree Class Implementation ****************************/

template <class T, class S>
void AVLTree<T,S>::destroyTreeAux(std::shared_ptr<Node<T,S>> subtree_root)
{
    if(!subtree_root) return;
    T key = subtree_root->getKey();
    std::shared_ptr<Node<T,S>> right_son = subtree_root->getRightSon();
    std::shared_ptr<Node<T,S>> left_son = subtree_root->getLeftSon();
    destroyTreeAux(left_son);
    removeNode(key);
    destroyTreeAux(right_son);
}

//CHANGESSSSSSS DESTRUCTOR

template <class T, class S>
AVLTree<T,S>::~AVLTree()
{
    destroyTreeAux(root);
    biggest_node = nullptr;
    root = nullptr;
}


/*/
 * searches the tree for the given value
 * if found returns pointer to the node with given key
 * else returns nullptr
 */
template<class T,class S>
std::shared_ptr<Node<T,S>> AVLTree<T,S>::findNode(T key) const
{
    if(root == nullptr)
    {
        return nullptr;
    }
    std::shared_ptr<Node<T,S>> curr = root;
    while(curr != nullptr)
    {
        if(curr->getKey() == key)
        {
            return curr;
        }
        else if(curr->getKey() > key)
        {
            curr = curr->getLeftSon();
        }
        else
        {
            curr = curr->getRightSon();
        }
    }
    return nullptr; //check
}

template<class T,class S>
void AVLTree<T,S>::setBiggest(std::shared_ptr<Node<T, S>> node)
{
    this->biggest_node = node;
}

template<class T,class S>
std::shared_ptr<Node<T,S>> AVLTree<T,S>::get_second_biggest()
{
    /*std::shared_ptr<AVLTree<T,S>> temp_tree = std::make_shared<AVLTree<T,S>>(*this);
    std::shared_ptr<Node<T,S>> max = temp_tree->findBiggest();
    temp_tree->removeNode(max->getKey());
    return temp_tree->findBiggest();*/
    /*if(!root) return nullptr;
    auto curr = root;
    if(!root->getRightSon()) //root is leaf or has only left son
    {
        if(root->isLeaf()) return nullptr;
        //has only left son
        return root->getLeftSon();
    }
    //root has at least two children
    while(curr->getRightSon()->getRightSon())
    {
        curr = curr->getRightSon();
    }
    return curr;*/
    if(!biggest_node) return nullptr;
    if(biggest_node->getLeftSon()) return biggest_node->getLeftSon();
    std::shared_ptr<Node<T,S>> biggest_parent = biggest_node->getParent();
    if(!biggest_parent) return nullptr;
    //has no children but has a parent
    return biggest_parent;
}

/* returns false if key already exists
 * returns true with successful insertion
 * */
template<class T,class S>
bool AVLTree<T,S>::insertNode(const T& key, const S& value)
{
    if(findNode(key) != nullptr) //key already exists, can't insert
    {
        return false;
    }
    std::shared_ptr<Node<T,S>> new_node;
    try
    {
        new_node = std::make_shared<Node<T,S>>(key, value);
    }
    catch (std::bad_alloc)
    {
        return false;
    }
    if(root == nullptr)
    {
        root = new_node;
        this->setBiggest(new_node);
        return true;
    }
    std::shared_ptr<Node<T,S>> new_parent; //new_node will be new_parent's son
    std::shared_ptr<Node<T,S>> curr = root;
    bool direction = true; //true - new_node will be new_parent's right son. false - will be a left son/
    while(curr)
    {
        new_parent = curr;
        if(curr->getKey() > key)
        {
            curr = curr->getLeftSon();
            direction = false;
        }

        else if(curr->getKey() < key)
        {
            curr = curr->getRightSon();
            direction = true;
        }
    }
    if(direction) //insert right
    {
        new_parent->setRightSon(new_node);
    }

    else //insert left
    {
        new_parent->setLeftSon(new_node);
    }
    new_node->setParent(new_parent);

    updateAncestorHeightsAndBF(new_node);
    curr = new_node;
    curr = goToFirstUnbalanced(curr);
    balance(curr);

    std::shared_ptr<Node<T,S>> curr_biggest = this->biggest_node;
    if(curr_biggest->getKey() < new_node->getKey() )
    {
        this->setBiggest(new_node);
    }
    return true;
}


template <class T, class S>
bool AVLTree<T,S>::removeNode(const T& key) {
    std::shared_ptr<Node<T, S>> wanted_node = findNode(key);

    if (wanted_node == nullptr)
    {
        return false;
    }

    if(biggest_node && wanted_node->getKey() == this->biggest_node->getKey() && getTreeSize() > 1)
    {
        biggest_node = this->get_second_biggest();
    }

    // Check if wanted node is a left son or a right son:
    bool direction = false; // false is left son, true is right son
    std::shared_ptr<Node<T, S>> wanted_parent = wanted_node->getParent();

    // Check if wanted_node is root
    bool wanted_is_root = false;
    if (wanted_parent == nullptr)
    {
        wanted_is_root = true;
    }

    if (!wanted_is_root && wanted_parent->getRightSon() == wanted_node) {
        direction = true;
    }

    if (wanted_node->isLeaf()) {
        if (wanted_is_root)
        {
            root = nullptr;
            setBiggest(nullptr);
            return true;
        }

        if (direction) { // right son
            wanted_parent->setRightSon(nullptr);
        } else {
            wanted_parent->setLeftSon(nullptr);
        }

        updateAncestorHeightsAndBF(wanted_parent);

        std::shared_ptr<Node<T, S>> ptr = wanted_parent;
        while (ptr) {
            if (abs(ptr->getBF()) == 2) {
                break;
            }
            ptr = ptr->getParent();
        }

        while (ptr) {
            balance(ptr);
            ptr = ptr->getParent();
        }
        wanted_node->detach();

        return true;
    }

    // If wanted node has only one son, let wanted node's parent point to wanted_node's son
    bool left_son_exists = (wanted_node->getLeftSon() != nullptr);
    bool right_son_exists = (wanted_node->getRightSon() != nullptr);

    if (!left_son_exists || !right_son_exists) {
        std::shared_ptr<Node<T, S>> child;
        if (!right_son_exists) { // right son is null - wanted_node has only left son
            child = wanted_node->getLeftSon();
        } else { // left son is null - wanted_node has only right son
            child = wanted_node->getRightSon();
        }

        child->setParent(wanted_parent);

        if (!wanted_is_root) {
            if (direction) {
                wanted_parent->setRightSon(child);
            } else {
                wanted_parent->setLeftSon(child);
            }
        } else {
            root = child;
        }

        updateAncestorHeightsAndBF(wanted_parent);

        std::shared_ptr<Node<T, S>> ptr = wanted_parent;
        while (ptr) {
            if (abs(ptr->getBF()) == 2) {
                break;
            }
            ptr = ptr->getParent();
        }

        while (ptr) {
            balance(ptr);
            ptr = ptr->getParent();
        }

        wanted_node->detach();

        return true;
    }

    // Wanted node has two children: supremum_child is the following inorder child
    std::shared_ptr<Node<T, S>> wanted_right_son = wanted_node->getRightSon();
    std::shared_ptr<Node<T, S>> wanted_left_son = wanted_node->getLeftSon();
    std::shared_ptr<Node<T, S>> supremum_child = wanted_right_son;

    while (supremum_child->getLeftSon() != nullptr) {
        supremum_child = supremum_child->getLeftSon();
    }

    std::shared_ptr<Node<T, S>> supremum_parent = supremum_child->getParent();
    std::shared_ptr<Node<T, S>> supremum_right_son = supremum_child->getRightSon();

    if (supremum_parent != wanted_node) {
        supremum_parent->setLeftSon(supremum_right_son);
        if (supremum_right_son != nullptr) {
            supremum_right_son->setParent(supremum_parent);
        }
    }

    supremum_child->setParent(wanted_parent);
    supremum_child->setLeftSon(wanted_left_son);
    wanted_left_son->setParent(supremum_child);

    if (supremum_child != wanted_right_son) {
        supremum_child->setRightSon(wanted_right_son);
        wanted_right_son->setParent(supremum_child);
    }

    if (!wanted_is_root) {
        if (direction) {
            wanted_parent->setRightSon(supremum_child);
        } else {
            wanted_parent->setLeftSon(supremum_child);
        }
    } else {
        root = supremum_child;
    }

    wanted_node->detach();

    std::shared_ptr<Node<T, S>> first_to_update = supremum_child->getRightSon();
    if (!first_to_update) {
        first_to_update = supremum_child;
    }

    while (first_to_update != nullptr && first_to_update->getLeftSon() != nullptr) {
        first_to_update = first_to_update->getLeftSon();
    }

    updateAncestorHeightsAndBF(first_to_update);
    supremum_child->setHeight(supremum_child->calcHeight());
    supremum_child->setBF(supremum_child->calcBalanceFactor());

    std::shared_ptr<Node<T, S>> ptr = first_to_update;
    while (ptr) {
        if (abs(ptr->getBF()) == 2) {
            break;
        }
        ptr = ptr->getParent();
    }

    while (ptr) {
        balance(ptr);
        ptr = ptr->getParent();
    }

    if (wanted_is_root) {
        auto curr = supremum_child;
        while (curr->getParent() != nullptr) {
            curr = curr->getParent();
        }
        root = curr;
    }

    return true;
}

template<class T,class S>
std::shared_ptr<Node<T,S>> AVLTree<T,S>::getMax()
{
    return this->biggest_node;
}

template<class T,class S>
void AVLTree<T,S>::rotateLeft(std::shared_ptr<Node<T,S>> subtree_root)
{
    if(subtree_root == nullptr) return;
    auto subtree_parent = subtree_root->getParent();
    auto subtree_right = subtree_root->getRightSon();
    if(subtree_right != nullptr)
    {
        subtree_right->setParent(subtree_parent);
        subtree_root->setRightSon(subtree_right->getLeftSon());
        if(subtree_right->getLeftSon() != nullptr)
        {
            subtree_right->getLeftSon()->setParent(subtree_root);
        }
    }
    if(subtree_parent != nullptr)
    {
        //find out if subtree_root is a left son or a right son:
        if(subtree_parent->getRightSon() == subtree_root)
        {
            subtree_parent->setRightSon(subtree_right);
        }
        else if(subtree_parent->getLeftSon() == subtree_root)
        {
            subtree_parent->setLeftSon(subtree_right);
        }
    }
    subtree_root->setParent(subtree_right);
    if(subtree_right != nullptr)
    {
        subtree_right->setLeftSon(subtree_root);
    }
    updateAncestorHeightsAndBF(subtree_root);
}

template<class T,class S>
void AVLTree<T,S>::rotateRight(std::shared_ptr<Node<T,S>> subtree_root)
{
    if(subtree_root == nullptr) return;
    auto subtree_parent = subtree_root->getParent();
    auto subtree_left = subtree_root->getLeftSon();
    if(subtree_left != nullptr)
    {
        subtree_left->setParent(subtree_parent);
        subtree_root->setLeftSon(subtree_left->getRightSon());
        if(subtree_left->getRightSon() != nullptr)
        {
            subtree_left->getRightSon()->setParent(subtree_root);
        }
    }
    if(subtree_parent != nullptr)
    {
        //find out if subtree_root is a left son or a right son:
        if(subtree_parent->getRightSon() == subtree_root)
        {
            subtree_parent->setRightSon(subtree_left);
        }
        else if(subtree_parent->getLeftSon() == subtree_root)
        {
            subtree_parent->setLeftSon(subtree_left);
        }
    }
    subtree_root->setParent(subtree_left);
    if(subtree_left != nullptr)
    {
        subtree_left->setRightSon(subtree_root);
    }
    updateAncestorHeightsAndBF(subtree_root);
}

template<class T,class S>
void AVLTree<T,S>::rotateLL(std::shared_ptr<Node<T,S>> subtree_root)
{
    if(!subtree_root) return;
    rotateRight(subtree_root);
}

template<class T,class S>
void AVLTree<T,S>::rotateRR(std::shared_ptr<Node<T,S>> subtree_root)
{
    if(!subtree_root) return;
    rotateLeft(subtree_root);
}

template<class T,class S>
void AVLTree<T,S>::rotateLR(std::shared_ptr<Node<T,S>> subtree_root)
{
    if(!subtree_root) return;
    rotateLeft(subtree_root->getLeftSon());
    rotateRight(subtree_root);
}

template<class T,class S>
void AVLTree<T,S>::rotateRL(std::shared_ptr<Node<T,S>> subtree_root)
{
    if(!subtree_root) return;
    rotateRight(subtree_root->getRightSon());
    rotateLeft(subtree_root);
}

template<class T,class S>
void AVLTree<T,S>::balance(std::shared_ptr<Node<T,S>> subtree_root)
{
    if(!subtree_root) return;
    if(subtree_root->getBF() == POS_FACTOR)
    {
        if(subtree_root->getLeftSon()->getBF() >= 0)
        {
            rotateLL(subtree_root);
        }
        else
        {
            rotateLR(subtree_root);
        }
    }
    else if(subtree_root->getBF() == NEG_FACTOR)
    {
        if (subtree_root->getRightSon()->getBF() <=0)
        {
            {
                rotateRR(subtree_root);
            }
        }
        else
        {
            rotateRL(subtree_root);
        }
    }
    else return;
    if(root == subtree_root) //if root is part of rotation:
    {
        root = root->getParent();
    }
}

template<class T,class S>
template <class DoSomething, class Param1, class Param2, class Param3>
void AVLTree<T,S>::inOrder(std::shared_ptr<Node<T,S>> subtree_root, DoSomething doSomething, Param1 param1,
             Param2 param2, Param3 param3)
{
    if(subtree_root == nullptr)
    {
        return;
    }
    std::shared_ptr<Node<T,S>> left_son = subtree_root->getLeftSon();
    std::shared_ptr<Node<T,S>> right_son = subtree_root->getRightSon();
    inOrder(left_son, doSomething, param1, param2, param3);
    doSomething(subtree_root, param1, param2, param3);
    inOrder(right_son, doSomething, param1, param2, param3);
}

template<class T,class S>
template <class DoSomething, class Param1, class Param2, class Param3>
void AVLTree<T,S>::preOrder(std::shared_ptr<Node<T,S>> subtree_root, DoSomething doSomething, Param1 param1,
              Param2 param2, Param3 param3)
{
    if(subtree_root == nullptr)
    {
        return;
    }
    doSomething(subtree_root, param1, param2);
    preOrder(subtree_root->getLeftSon(), doSomething, param1, param2, param3);
    preOrder(subtree_root->getRightSon(), doSomething, param1, param2, param3);
}

template<class T,class S>
template <class DoSomething, class Param1, class Param2, class Param3>
void AVLTree<T,S>::postOrder(std::shared_ptr<Node<T,S>> subtree_root, DoSomething doSomething, Param1 param1,
               Param2 param2, Param3 param3)
{
    if(subtree_root == nullptr)
    {
        return;
    }
    postOrder(subtree_root->getLeftSon(), doSomething, param1, param2, param3);
    postOrder(subtree_root->getRightSon(), doSomething, param1, param2, param3);
    doSomething(subtree_root, param1, param2, param3);
}

template<class T,class S>
std::shared_ptr<Node<T,S>> AVLTree<T,S>::findBiggest()
{
    if(root == nullptr)
    {
        return nullptr;
    }
    auto curr = root;
    while(curr->getRightSon() != nullptr)
    {
        curr = curr->getRightSon();
    }
    return curr;
}

template<class T,class S>
void AVLTree<T,S>::updateAncestorHeightsAndBF(std::shared_ptr<Node<T,S>> subtree_root)
{
    auto curr = subtree_root;
    while(curr != nullptr)
    {
        int left_son_height = (curr->getLeftSon() == nullptr) ? -1 : curr->getLeftSon()->getHeight();
        int right_son_height = (curr->getRightSon() == nullptr) ? -1 : curr->getRightSon()->getHeight();
        int ptr_height = (left_son_height > right_son_height) ? left_son_height + 1 : right_son_height + 1;
        curr->setHeight(ptr_height);
        curr->setBF(curr->calcBalanceFactor());
        curr = curr->getParent();
    }
}

//CHECK AND TEST!
template<class T,class S>
std::shared_ptr<Node<T,S>> AVLTree<T,S>::goToFirstUnbalanced(std::shared_ptr<Node<T,S>> ptr)
{
    while(ptr)
    {
        if(abs(ptr->getBF()) == POS_FACTOR)
        {
            return ptr;
        }
        ptr = ptr->getParent();
    }
    return ptr;
}

template<class T,class S>
bool AVLTree<T,S>::isRightSon(std::shared_ptr<Node<T,S>> node)
{
    auto parent = node->getParent();
    if(!parent) return false; //just in case
    if(parent->getRightSon() == node)
    {
        return true;
    }
    return false;
}

/*
template<class T, class S>
void AVLTree<T,S>::buildEmptyTree(int h,std::shared_ptr<Node<T,S>> new_sub_root)
{
    if(h == 0)
    {
        return;
    }
    if (new_sub_root == nullptr)
    {
        new_sub_root = std::make_shared<Node<T,S>>();
    }
    std::shared_ptr<Node<T,S>> new_empty_left_son = std::make_shared<Node<T,S>>();
    std::shared_ptr<Node<T,S>> new_empty_right_son = std::make_shared<Node<T,S>>();
    new_empty_right_son->setParent(new_sub_root);
    new_empty_left_son->setParent(new_sub_root);
    new_sub_root->setLeftSon(new_empty_left_son);
    new_sub_root->setRightSon(new_empty_right_son);
    if (h > 0)
    {
        buildEmptyTree(h-1,new_empty_left_son);
        buildEmptyTree(h-1,new_empty_right_son);
    }
}
 */



//*********************************** dim helping func***********************************//

template<class T,class S>
std::shared_ptr<Node<T,S>> AVLTree<T,S>::getNextInorder(std::shared_ptr <Node<T, S>> node)
{
    std::shared_ptr <Node<T, S>> curr = node->getRightSon();
    while(curr->getLeftSon() != nullptr)
    {
        curr = curr->getLeftSon();
    }
    return curr;
}


template<class T,class S>
bool AVLTree<T,S>::hasOneSon(std::shared_ptr <Node<T, S>> node)
{
    //should check if node is null!
    bool hasLeftSon = ((node->getRightSon() == nullptr) && (node->getLeftSon()!= nullptr));
    bool hasRightSon = ((node->getRightSon() != nullptr) && (node->getLeftSon()== nullptr));
    return (hasLeftSon || hasRightSon);
}


template<class T,class S>
std::shared_ptr<Node<T,S>> AVLTree<T,S>::getRoot() const
{
    return root;
}

template<class T,class S>
int AVLTree<T,S>::getTreeSizeAux(std::shared_ptr<Node<T,S>> node)
{
    if(!node) return 0;
    int left_num = getTreeSizeAux(node->getLeftSon());
    int right_num = getTreeSizeAux(node->getRightSon());
    return left_num + right_num + 1;
}


template<class T,class S>
int AVLTree<T,S>::getTreeSize()
{
    return getTreeSizeAux(root);
}


#endif //HW1WETDS_AVLTREE_H
