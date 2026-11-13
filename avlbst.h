#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);
    void rotateRight(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* child = node->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();

    node->setLeft(child->getRight());
    if (child->getRight() != NULL) {
        child->getRight()->setParent(node);
    }
    
    child->setRight(node);
    node->setParent(child);

    child->setParent(parent);
    if (parent == NULL) {
        this->root_ = child;
    } else if (parent->getLeft() == node) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* child = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();

    node->setRight(child->getLeft());
    if (child->getLeft() != NULL) {
        child->getLeft()->setParent(node);
    }

    child->setLeft(node);
    node->setParent(child);

    child->setParent(parent);
    if (parent == NULL) {
        this->root_ = child;
    } else if (parent->getLeft() == node) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) {
    if (p == NULL || p->getParent() == NULL) return;
    AVLNode<Key, Value>* g = p->getParent();

    // Assume Left heavy is -1, Right heavy is +1
    if (g->getLeft() == p) {
        g->updateBalance(-1);
        if (g->getBalance() == 0) return;
        if (g->getBalance() == -1) {
            insertFix(g, p);
        } else if (g->getBalance() == -2) {
            if (p->getBalance() == -1) {
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            } else {
                rotateLeft(p);
                rotateRight(g);
                if (n->getBalance() == -1) { p->setBalance(0); g->setBalance(1); }
                else if (n->getBalance() == 0) { p->setBalance(0); g->setBalance(0); }
                else if (n->getBalance() == 1) { p->setBalance(-1); g->setBalance(0); }
                n->setBalance(0);
            }
        }
    } else {
        g->updateBalance(1);
        if (g->getBalance() == 0) return;
        if (g->getBalance() == 1) {
            insertFix(g, p);
        } else if (g->getBalance() == 2) {
            if (p->getBalance() == 1) {
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            } else {
                rotateRight(p);
                rotateLeft(g);
                if (n->getBalance() == 1) { p->setBalance(0); g->setBalance(-1); }
                else if (n->getBalance() == 0) { p->setBalance(0); g->setBalance(0); }
                else if (n->getBalance() == -1) { p->setBalance(1); g->setBalance(0); }
                n->setBalance(0);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff) {
    if (n == NULL) return;
    AVLNode<Key, Value>* p = n->getParent();
    int8_t ndiff = 0;
    if (p != NULL) {
        ndiff = (p->getLeft() == n) ? 1 : -1;
    }

    if (n->getBalance() + diff == -2) {
        AVLNode<Key, Value>* c = n->getLeft();
        if (c->getBalance() == -1) {
            rotateRight(n);
            n->setBalance(0); c->setBalance(0);
            removeFix(p, ndiff);
        } else if (c->getBalance() == 0) {
            rotateRight(n);
            n->setBalance(-1); c->setBalance(1);
        } else if (c->getBalance() == 1) {
            AVLNode<Key, Value>* g = c->getRight();
            rotateLeft(c); rotateRight(n);
            if (g->getBalance() == 1) { n->setBalance(0); c->setBalance(-1); }
            else if (g->getBalance() == 0) { n->setBalance(0); c->setBalance(0); }
            else if (g->getBalance() == -1) { n->setBalance(1); c->setBalance(0); }
            g->setBalance(0);
            removeFix(p, ndiff);
        }
    } else if (n->getBalance() + diff == 2) {
        AVLNode<Key, Value>* c = n->getRight();
        if (c->getBalance() == 1) {
            rotateLeft(n);
            n->setBalance(0); c->setBalance(0);
            removeFix(p, ndiff);
        } else if (c->getBalance() == 0) {
            rotateLeft(n);
            n->setBalance(1); c->setBalance(-1);
        } else if (c->getBalance() == -1) {
            AVLNode<Key, Value>* g = c->getLeft();
            rotateRight(c); rotateLeft(n);
            if (g->getBalance() == -1) { n->setBalance(0); c->setBalance(1); }
            else if (g->getBalance() == 0) { n->setBalance(0); c->setBalance(0); }
            else if (g->getBalance() == 1) { n->setBalance(-1); c->setBalance(0); }
            g->setBalance(0);
            removeFix(p, ndiff);
        }
    } else if (n->getBalance() + diff == -1 || n->getBalance() + diff == 1) {
        n->setBalance(n->getBalance() + diff);
    } else if (n->getBalance() + diff == 0) {
        n->setBalance(0);
        removeFix(p, ndiff);
    }
}
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO // done
    if (this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = NULL;

    while (curr != NULL) {
        parent = curr;
        if (new_item.first < curr->getKey()) {
            curr = curr->getLeft();
        } else if (curr->getKey() < new_item.first) {
            curr = curr->getRight();
        } else {
            curr->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
        parent->updateBalance(-1);
    } else {
        parent->setRight(newNode);
        parent->updateBalance(1);
    }

    if (parent->getBalance() != 0) {
        insertFix(parent, newNode);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODOdone
    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (target == NULL) return;

    if (target->getLeft() != NULL && target->getRight() != NULL) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(target));
        nodeSwap(target, pred);
    }

    AVLNode<Key, Value>* child = (target->getLeft() != NULL) ? target->getLeft() : target->getRight();
    AVLNode<Key, Value>* parent = target->getParent();
    int8_t diff = 0;

    if (parent != NULL) {
        diff = (parent->getLeft() == target) ? 1 : -1;
    }

    if (child != NULL) {
        child->setParent(parent);
    }

    if (parent == NULL) {
        this->root_ = child;
    } else if (parent->getLeft() == target) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }

    delete target;
    removeFix(parent, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
