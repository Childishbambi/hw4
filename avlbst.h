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
    void rotateLeft(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n){
      if(p->getParent() != NULL){

        if(p == p->getParent()->getLeft()){
          p->getParent()->setLeft(n);
        } else {
          p->getParent()->setRight(n);
        }
      } else {
        this->root_ = n;
      }

      n->setParent(p->getParent());
      p->setParent(n);
      p->setRight(n->getLeft());

      if(n->getLeft() != NULL){
        n->getLeft()->setParent(p);
      }
      n->setLeft(p);
    }

    void rotateRight(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n){
      if(p->getParent() != NULL){

        if(p == p->getParent()->getLeft()){
          p->getParent()->setLeft(n);
        } else {
          p->getParent()->setRight(n);
        }

      } else {
        this->root_ = n;
      }

      n->setParent(p->getParent());
      p->setParent(n);
      p->setLeft(n->getRight());

      if(n->getRight() != NULL){
       n->getRight()->setParent(p);
      }
      n->setRight(p);
    }

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL){
       this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
       return;
    } else {
      AVLNode<Key, Value> *c = (AVLNode<Key,Value>*)(this->root_);
      AVLNode<Key, Value> *p = NULL;

      while(c != NULL){
        p = c;

        if(new_item.first == c->getKey()){
            c->setValue(new_item.second);
            return;
        } else if(new_item.first < c->getKey()){
            c = c->getLeft();
        } else {
            c = c->getRight();
        }
      }

      AVLNode<Key, Value> *insertN = new AVLNode<Key, Value>(new_item.first, new_item.second, p);
      if(new_item.first < p->getKey()){
        p->setLeft(insertN);
      } else {
        p->setRight(insertN);
      }

      insertN->setBalance(0);
      c = insertN;
      while(p != NULL){

        if(c == p->getLeft()){
            p->updateBalance(-1);
        } else {
            p->updateBalance(1);
        }

        int8_t curr = p->getBalance();
        if (curr == 0){
          return;

        } else if(curr == 1 || curr == -1){
          c = p;
          p = p->getParent();

        } else {
          AVLNode<Key, Value> *g = p->getParent();
          AVLNode<Key, Value> *root = NULL;

          if (curr > 1){
            AVLNode<Key, Value>* child = p->getRight();

            if(child->getBalance() >= 0){
              rotateLeft(p, child);
              p->setBalance(0);
              child->setBalance(0);
              root = child;

            } else {
              AVLNode<Key, Value>* gc = child->getLeft();
              int8_t gcBal = gc->getBalance();
              rotateRight(child, gc);
              rotateLeft(p, gc);

              if(gcBal == -1){
                p->setBalance(0);
                child->setBalance(1);

              } else if(gcBal == 0){
                p->setBalance(0);
                child->setBalance(0);

              } else {
                p->setBalance(-1);
                child->setBalance(0);
              }

              gc->setBalance(0);
              root = gc;
            }
          } else if (curr < -1){
            AVLNode<Key, Value> *child = p->getLeft();

            if(child->getBalance() <= 0){
              rotateRight(p, child);
              p->setBalance(0);
              child->setBalance(0);
              root = child;

            } else {
              AVLNode<Key, Value> *gc = child->getRight();
              int8_t gcBal = gc->getBalance();
              rotateLeft(child, gc);
              rotateRight(p, gc);

              if(gcBal == 1){
                p->setBalance(0);
                child->setBalance(-1);

              } else if(gcBal == 0){
                p->setBalance(0);
                child->setBalance(0);

              } else {
                p->setBalance(1);
                child->setBalance(0);
              }

              gc->setBalance(0);
              root = gc;
            }
          }

          if(g == NULL){
            this->root_ = root;
          } else if (p == g->getLeft()){
            g->setLeft(root);
          } else {
            g->setRight(root);
          }

          root->setParent(g);
          return;
      }
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(this->root_ == NULL){
      return;
    }
    if(this->internalFind(key) == NULL){
      return;
    }

    AVLNode<Key, Value> *delCurr = (AVLNode<Key, Value>*)(this->internalFind(key));
    if(delCurr->getLeft() != NULL && delCurr->getRight() != NULL){
      AVLNode<Key, Value> *pred = (AVLNode<Key, Value>*)this->predecessor(delCurr);
      nodeSwap(delCurr, pred);
    }

    AVLNode<Key, Value> *c;
    AVLNode<Key, Value> *p = delCurr->getParent();
    if(delCurr->getLeft() != NULL){
      c = delCurr->getLeft();
    } else {
      c = delCurr->getRight();
    }

    if(p != NULL){
      if(delCurr == p->getLeft()){
        p->setLeft(c);
      } else {
        p->setRight(c);
      }
    } else {
      this->root_ = c;
    }

    if(c != NULL){
      c->setParent(p);
    }
    delete delCurr;
    return;
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
