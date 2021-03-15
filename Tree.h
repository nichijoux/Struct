#pragma once
#ifndef _TREE_H
#define _TREE_H
using std::max;

template<class T>
struct TreeNode
{
	T val;					//通用val
	int height;				//AVL树使用
	int color;				//红黑树使用
	TreeNode* left;
	TreeNode* right;

	TreeNode() = default;
	TreeNode(T x) : val(x), height(0), color(0), left(nullptr), right(nullptr) {}
	TreeNode(T x, TreeNode* left, TreeNode* right) : val(x), height(0), color(0), left(left), right(right) {}
};

template<class T>
class Tree
{
protected:
	TreeNode<T>* root;				//根节点
	int NodeSize;					//Tree中的节点数量
	//前序遍历的辅助函数(TreeNode* 可更改node的值)
	void preOrderHelp(TreeNode<T>* root, void(*function)(TreeNode<T>* node));
	//中序遍历的辅助函数(TreeNode* 可更改node的值)
	void inOrderHelp(TreeNode<T>* root, void(*function)(TreeNode<T>* node));
	//后序遍历的辅助函数(TreeNode* 可更改node的值)
	void backOrderHelp(TreeNode<T>* root, void(*function)(TreeNode<T>* node));
	//得到树的高度的辅助函数
	int get_Height_Help(TreeNode<T>* root)const;
public:
	//构造函数
	Tree() { root = nullptr; NodeSize = 0; }
	virtual ~Tree() = default;
	//前序遍历
	void preOrder(void(*function)(TreeNode<T>* node)) { preOrderHelp(root, function); };
	//中序遍历
	void inOrder(void(*function)(TreeNode<T>* node)) { inOrderHelp(root, function); }
	//后序遍历
	void backOrder(void(*function)(TreeNode<T>* node)) { backOrderHelp(root, function); }
	//得到树的高度
	int getHeight() { return get_Height_Help(root); }
	//得到节点数
	int getNodeSize()const { return NodeSize; }
};

#endif // Tree.h


//函数实现

//前序遍历的辅助函数
template<class T>
void Tree<T>::preOrderHelp(TreeNode<T>* root, void(*function)(TreeNode<T>* node))
{
	if (root == nullptr) { return; }
	function(root);
	preOrderHelp(root->left, function);
	preOrderHelp(root->right, function);
}

//中序遍历的辅助函数
template<class T>
void Tree<T>::inOrderHelp(TreeNode<T>* root, void(*function)(TreeNode<T>* node))
{
	if (root == nullptr) { return; }
	inOrderHelp(root->left, function);
	function(root);
	inOrderHelp(root->right, function);
}

//后序遍历的辅助函数
template<class T>
void Tree<T>::backOrderHelp(TreeNode<T>* root, void(*function)(TreeNode<T>* node))
{
	if (root == nullptr) { return; }
	backOrderHelp(root->left, function);
	backOrderHelp(root->right, function);
	function(root);
}

//得到树的高度的辅助函数
template<class T>
int Tree<T>::get_Height_Help(TreeNode<T>* root)const
{
	if (root == nullptr) { return 0; }
	return max(get_Height_Help(root->left), get_Height_Help(root->right)) + 1;
}
