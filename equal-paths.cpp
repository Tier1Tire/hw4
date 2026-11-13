#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkLeafDepths(Node* node, int currentDepth, int& expectedDepth);

bool equalPaths(Node * root)
{
    // Add your code below
    int expectedDepth = -1;
    return checkLeafDepths(root,0,expectedDepth);
}

bool checkLeafDepths(Node* node, int currentDepth, int& expectedDepth)
{
  // base case 1
  if(node == nullptr){
    return true;
  } 

  //base base 2
  if (node->left == nullptr && node->right == nullptr){
    if(expectedDepth == -1){
      expectedDepth = currentDepth;
      return true;
    }
    return currentDepth == expectedDepth;
  }

  //recursion
  return checkLeafDepths(node->left, currentDepth+1, expectedDepth) && checkLeafDepths(node->right, currentDepth+1, expectedDepth);

}

