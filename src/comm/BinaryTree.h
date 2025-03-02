#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

#include "Common.h"

///////////////
//  DEFINES  //
///////////////

#define BinaryTree_preOrderTraversal(bt, call) BinaryTree_nodePreOrderTraversal(((BinaryTree*)bt)->headNode, call)
#define BinaryTree_postOrderTraversal(bt, call) BinaryTree_nodePostOrderTraversal(((BinaryTree*)bt)->headNode, call)
#define BinaryTree_inOrderTraversal(bt, call) BinaryTree_nodeInOrderTraversal(((BinaryTree*)bt)->headNode, call)

/////////////
//  TYPES  //
/////////////

typedef struct TreeNode
{
  unsigned int nodeCount;
  void * data;
  struct TreeNode * left;
  struct TreeNode * right;
} TreeNode;

typedef struct BinaryTree
{
  U4 nodeCount;
  CompareFunction compFunc;
  FreeDataFunction freeFunc;
  TreeNode * headNode;
  bool passByVal;
  U4 dataLen;
} BinaryTree;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

BinaryTree * BinaryTree_init(U4 dataLen, bool passByVal, CompareFunction compFunc, FreeDataFunction freeFunc);
void BinaryTree_free(BinaryTree * bt);

void BinaryTree_nodePreOrderTraversal(TreeNode * tn, CallbackFunction callback, void * args);
void BinaryTree_nodePostOrderTraversal(TreeNode * tn, CallbackFunction callback, void * args);
void BinaryTree_nodeInOrderTraversal(TreeNode * tn, CallbackFunction callback, void * args);

bool BinaryTree_insert(BinaryTree * bt, void * data);
bool BinaryTree_remove(BinaryTree * bt, void * data);

#endif
