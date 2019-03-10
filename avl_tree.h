#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "dsexceptions.h"
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

// AvlTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

/*
//                Functions implemeted by Me
// int NumOfNodes()             --> returns the number of nodes in the tree
// float AverageDepthOfTree()   -->  returns the average depth (i.e., depth of all nodes/number of nodes)
// float RatioOfAverageDepth()  --> prints the ratio of the average depth to log(n) (Base 2)
// bool RemoveEveryOther ()     --> removes every other sequence from sequences.txt from the tree and counts the number of recursion calls.
// bool find()                  --> searches the tree for each string in the sequences.txt file and counts the number of recursive calls
// void PrintEnzymesForUserSequence()  --> Finds the object with the matching sequence and prints out the enzymes associated with that sequence
Under private:
// float AccumulateDepthInfo()  --> counts the depth of all nodes // helper for AverageDepthOfTree()
*/

/*
Slight modificaion under the Professor insert() to merge enzymes in case a sequence was already in one of the nodes tree
*/

template <typename Comparable>
class AvlTree
{
  public:
    AvlTree( ) : root{ nullptr }
      { }

    AvlTree( const AvlTree & rhs ) : root{ nullptr }
    {
        root = clone( rhs.root );
    }

    AvlTree( AvlTree && rhs ) : root{ rhs.root }
    {
        rhs.root = nullptr;
    }

    ~AvlTree( )
    {
        makeEmpty( );
    }

    /**
     * Deep copy.
     */
    AvlTree & operator=( const AvlTree & rhs )
    {
        AvlTree copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    /**
     * Move.
     */
    AvlTree & operator=( AvlTree && rhs )
    {
        std::swap( root, rhs.root );

        return *this;
    }

    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMin( root )->element;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMax( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMax( root )->element;
    }

    /**
     * Returns true if x is found in the tree.
     */
    bool contains( const Comparable & x ) const
    {
        return contains( x, root );
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty( ) const
    {
        return root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree( ) const
    {
        if( isEmpty( ) )
            cout << "Empty tree" << endl;
        else
            printTree( root );
    }

    /**
     * Make the tree logically empty.
     */
    void makeEmpty( )
    {
        makeEmpty( root );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( const Comparable & x )
    {
        insert( x, root );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( Comparable && x )
    {
        insert( std::move( x ), root );
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove( const Comparable & x )
    {
        remove( x, root );
    }

    int NumOfNodes() const {
      return NumOfNodes(root);
    }

    // AccumulateDepthInfo is private
    float AverageDepthOfTree() const {
      float depth_info = AccumulateDepthInfo(root, 0);
      int total_nodes =  NumOfNodes();
      return AverageDepthOfTree(depth_info, static_cast<float>(total_nodes));
    }

    float RatioOfAverageDepth() const {
      float log_of_total_nodes = static_cast<float>(NumOfNodes());
      log_of_total_nodes = log(log_of_total_nodes)/log(2.0);
      float avg_depth = AverageDepthOfTree();
      return (avg_depth/log_of_total_nodes);
    }

    bool RemoveEveryOther(const std::string & x , int &calls) {
      return RemoveEveryOther(x, root, calls);
    }

    bool find(const std::string &x, int &calls) const {
      return find(x, root, calls);
    }

    void PrintEnzymesForUserSequence(const Comparable & x ) const {
      return PrintEnzymesForUserSequence(x, root);
    }

  private:
    struct AvlNode
    {
        Comparable element;
        AvlNode   *left;
        AvlNode   *right;
        int       height;

        AvlNode( const Comparable & ele, AvlNode *lt, AvlNode *rt, int h = 0 )
          : element{ ele }, left{ lt }, right{ rt }, height{ h } { }

        AvlNode( Comparable && ele, AvlNode *lt, AvlNode *rt, int h = 0 )
          : element{ std::move( ele ) }, left{ lt }, right{ rt }, height{ h } { }
    };

    AvlNode *root;

    /**
     * Internal method to counts all nodes in the tree.
     * t is the node that roots the subtree.
    */
    int NumOfNodes(AvlNode *t) const {
      if (t == nullptr)
        return 0;
      else
        return NumOfNodes(t->left) + NumOfNodes(t->right) + 1;
    }

    /**
     * Computes the average depth of our search tree (i.e., internal path length
     * divided by n)
    */
    float AverageDepthOfTree(const float &internal_path_length, const float &total_nodes) const {
      float result = internal_path_length/total_nodes;
      return result;
    }

    /**
     * t is the node that roots the subtree.
     * depth is the depth of each node
     * Counts the depth of all nodes in the tree
    */
    float AccumulateDepthInfo(AvlNode *t, float depth) const {
      if (t == nullptr)
        return 0;
      else
        return AccumulateDepthInfo(t->left, depth+1) + AccumulateDepthInfo(t->right, depth+1) + depth;
    }

    /**
     * x is the string to look for and then remove its node
     * t is the node that roots the subtree.
     * calls is the number of recursive calls it took to find it
    */
    bool RemoveEveryOther(const std::string &x, AvlNode * &t, int &calls) {
      calls++; // with each call we must add 1
      if (t == nullptr) {
        return false;
      }
      if (x < t->element.GetRecogSeq()) {
        return RemoveEveryOther( x, t->left, calls);
      }
      else if (t->element.GetRecogSeq() < x ) {
        return RemoveEveryOther( x, t->right, calls);
      }
      else if( t->left != nullptr && t->right != nullptr ) { // Two children
        t->element = findMin( t->right )->element;
        return RemoveEveryOther(t->element.GetRecogSeq(), t->right, calls);
      }
      else  { // match
        AvlNode *oldNode = t;
        t = ( t->left != nullptr ) ? t->left : t->right;
        delete oldNode;
        return true;
      }
    }

    /**
     * x is the string to look for
     * t is the node that roots the subtree.
     * calls is the number of recursive calls it took to find it
    */
    bool find(const std::string &x, AvlNode *t, int &calls) const {
      calls++; // with each call we must add 1
      if (t == nullptr)
        return false;
      else if( x < t->element.GetRecogSeq())
        return find( x, t->left, calls);
      else if( t->element.GetRecogSeq() < x )
        return find( x, t->right, calls);
      else
        return true; // Match
    }

    /**
     * x is the object to compare
     * t is the node that roots the subtree.
    */
    void PrintEnzymesForUserSequence(const Comparable & x, AvlNode *t ) const {
      if( t == nullptr )
        std::cout << "Not Found";
      else if( x < t->element )
        return PrintEnzymesForUserSequence( x, t->left );
      else if( t->element < x )
        return PrintEnzymesForUserSequence( x, t->right );
      else  // Match
        t->element.PrintVectorOfEnzymes();
    }

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( const Comparable & x, AvlNode * & t ) {
      if( t == nullptr ) {
        t = new AvlNode{ x, nullptr, nullptr };
      }
      else if( x < t->element ) {
        insert( x, t->left );
      }
      else if( t->element < x ) {
        insert( x, t->right );
      }
      else {
        t->element.Merge(x); // Merge if duplicate
      }
      balance( t );
    }

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( Comparable && x, AvlNode * & t ) {
      if( t == nullptr )
        t = new AvlNode{ std::move( x ), nullptr, nullptr };
      else if( x < t->element )
        insert( std::move( x ), t->left );
      else if( t->element < x )
        insert( std::move( x ), t->right );
      else
        t->element.Merge(x); // Merge if duplicate
      balance( t );
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, AvlNode * & t )
    {
        if( t == nullptr )
            return;   // Item not found; do nothing

        if( x < t->element )
            remove( x, t->left );
        else if( t->element < x )
            remove( x, t->right );
        else if( t->left != nullptr && t->right != nullptr ) // Two children
        {
            t->element = findMin( t->right )->element;
            remove( t->element, t->right );
        }
        else
        {
            AvlNode *oldNode = t;
            t = ( t->left != nullptr ) ? t->left : t->right;
            delete oldNode;
        }

        balance( t );
    }

    static const int ALLOWED_IMBALANCE = 1;

    // Assume t is balanced or within one of being balanced
    void balance( AvlNode * & t )
    {
        if( t == nullptr )
            return;

        if( height( t->left ) - height( t->right ) > ALLOWED_IMBALANCE ) {
            if( height( t->left->left ) >= height( t->left->right ) )
                rotateWithLeftChild( t );
            else
                doubleWithLeftChild( t );
        } else if( height( t->right ) - height( t->left ) > ALLOWED_IMBALANCE ) {
            if( height( t->right->right ) >= height( t->right->left ) )
                rotateWithRightChild( t );
            else
                doubleWithRightChild( t );
	        }
        t->height = max( height( t->left ), height( t->right ) ) + 1;
    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    AvlNode * findMin( AvlNode *t ) const
    {
        if( t == nullptr )
            return nullptr;
        if( t->left == nullptr )
            return t;
        return findMin( t->left );
    }

    /**
     * Internal method to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
    AvlNode * findMax( AvlNode *t ) const
    {
        if( t != nullptr )
            while( t->right != nullptr )
                t = t->right;
        return t;
    }

    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the tree.
     */
    bool contains( const Comparable & x, AvlNode *t ) const
    {
        if( t == nullptr ) {
          return false;
        }
        else if( x < t->element ) {
          return contains( x, t->left );
        }
        else if( t->element < x ) {
          return contains( x, t->right );
        }
        else {
          return true; // Match
          t->element.PrintVectorOfEnzymes();
        }
    }
/****** NONRECURSIVE VERSION*************************
    bool contains( const Comparable & x, AvlNode *t ) const
    {
        while( t != nullptr )
            if( x < t->element )
                t = t->left;
            else if( t->element < x )
                t = t->right;
            else
                return true;    // Match

        return false;   // No match
    }
*****************************************************/

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty( AvlNode * & t )
    {
        if( t != nullptr )
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = nullptr;
    }

    /**
     * Internal method to print a subtree rooted at t in sorted order.
     */
    void printTree( AvlNode *t ) const
    {
        if( t != nullptr )
        {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }
    }

    /**
     * Internal method to clone subtree.
     */
    AvlNode * clone( AvlNode *t ) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return new AvlNode{ t->element, clone( t->left ), clone( t->right ), t->height };
    }
        // Avl manipulations
    /**
     * Return the height of node t or -1 if nullptr.
     */
    int height( AvlNode *t ) const
    {
        return t == nullptr ? -1 : t->height;
    }

    int max( int lhs, int rhs ) const
    {
        return lhs > rhs ? lhs : rhs;
    }

    /**
     * Rotate binary tree node with left child.
     * For AVL trees, this is a single rotation for case 1.
     * Update heights, then set new root.
     */
    void rotateWithLeftChild( AvlNode * & k2 )
    {
        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), k2->height ) + 1;
        k2 = k1;
    }

    /**
     * Rotate binary tree node with right child.
     * For AVL trees, this is a single rotation for case 4.
     * Update heights, then set new root.
     */
    void rotateWithRightChild( AvlNode * & k1 )
    {
        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k2->height = max( height( k2->right ), k1->height ) + 1;
        k1 = k2;
    }

    /**
     * Double rotate binary tree node: first left child.
     * with its right child; then node k3 with new left child.
     * For AVL trees, this is a double rotation for case 2.
     * Update heights, then set new root.
     */
    void doubleWithLeftChild( AvlNode * & k3 )
    {
        rotateWithRightChild( k3->left );
        rotateWithLeftChild( k3 );
    }

    /**
     * Double rotate binary tree node: first right child.
     * with its left child; then node k1 with new right child.
     * For AVL trees, this is a double rotation for case 3.
     * Update heights, then set new root.
     */
    void doubleWithRightChild( AvlNode * & k1 )
    {
        rotateWithLeftChild( k1->right );
        rotateWithRightChild( k1 );
    }
};

#endif
