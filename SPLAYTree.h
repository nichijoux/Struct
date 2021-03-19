#ifndef SPLAYTREE_H
#define SPLAYTREE_H
#include "BSTree.h"

//伸展树
template<class T>
class SPLAY :virtual public BST<T>
{
private:
	// 旋转key对应的节点为根节点，并返回值为根节点。
	TreeNode<T>* splayHelp(TreeNode<T>* node, T& key);
public:
	//构造函数
	SPLAY() {}
	//BST会自行析构,~SPLAY()中不需要添加代码
	~SPLAY() {}

	// 旋转key对应的节点为根节点，并返回值为根节点。
	void splay(T& key);
};

template<class T>
TreeNode<T>* SPLAY<T>::splayHelp(TreeNode<T>* node, T& key)
{

}

#endif // SPLAYTREE_H
