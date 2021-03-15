#pragma once
#ifndef AVLTREE_H
#define AVLTREE_H
#include "BSTree.h"

//AVL树,AVL树是带平衡条件的BST
//AVL树是每个节点的左子树和右子树的高度最多差1的二叉查找树
template<class T>
class AVL :virtual public BST<T>
{
private:
	//得到某一节点的高度
	int GetNodeHeight(TreeNode<T>* node);
	//插入节点的辅助函数
	TreeNode<T>* InsertNode(TreeNode<T>* node, const T& val)final override;
	//删除节点的辅助函数
	TreeNode<T>* DeleteNode(TreeNode<T>* node, const T& val)final override;

	//转,转就完事儿
	//单左旋,插入时有RR插入,即向右子树的右孩子插入节点,导致不符合AVL树的定义
	TreeNode<T>* SingleRotateWithLeft(TreeNode<T>* preRoot);
	//单右旋,插入时有LL插入,即向左子树的左孩子插入节点,导致不符合AVL树的定义
	TreeNode<T>* SingleRotateWithRight(TreeNode<T>* preRoot);
	//双旋转实际是需要旋转两次
	//双左旋(R-L双旋转,RL插入(和单旋转不同))
	TreeNode<T>* DoubleRotateWithLeft(TreeNode<T>* preRoot);
	//双右旋(L-R双旋转,LR插入(和单旋转不同))
	TreeNode<T>* DoubleRotateWithRight(TreeNode<T>* preRoot);

	//声明使用间接基类的root,否则无法访问
	using Tree<T>::root;
	using Tree<T>::NodeSize;
public:
	//构造函数
	AVL() {}
	//析构函数(BST中已经有写好的析构了,不用再写了,否则报错)
	~AVL() {}
	//插入节点的函数(重写)
	void Insert(const T& val)override;
	//删除节点的函数(重写)
	void Delete(const T& val)override;
};

#endif // !AVLTREE_H

//得到某节点的高度
template<class T>
inline int AVL<T>::GetNodeHeight(TreeNode<T>* node)
{
	return node == nullptr ? 0 : node->height;
}

//旋转吧！雪月花
//单左旋,插入时有RR插入,即向右子树的右孩子插入节点,导致不符合AVL树的定义
template<class T>
TreeNode<T>* AVL<T>::SingleRotateWithLeft(TreeNode<T>* preRoot)
{
	//RR插入使得preRoot变为第一个不满足AVL树定义的节点
	TreeNode<T>* newRoot = preRoot->right;		//将newRoot节点作为新的root节点
	TreeNode<T>* RootR = newRoot->left;			//preRoot->right应该更新为RootR
	newRoot->left = preRoot;					//逆袭了
	preRoot->right = RootR;						//原root节点的右孩子现在找到了新的节点
	//更新各点高度
	preRoot->height = max(GetNodeHeight(preRoot->left), GetNodeHeight(preRoot->right)) + 1;
	newRoot->height = max(GetNodeHeight(newRoot->left), GetNodeHeight(newRoot->right)) + 1;
	return newRoot;
}

//单右旋,插入时有LL插入,即向左子树的左孩子插入节点,导致不符合AVL树的定义
template<class T>
TreeNode<T>* AVL<T>::SingleRotateWithRight(TreeNode<T>* preRoot)
{
	//LL插入使得preRoot变为第一个不满足AVL树定义的节点
	TreeNode<T>* newRoot = preRoot->left;		//将newRoot节点作为新的root节点
	TreeNode<T>* RootL = newRoot->right;		//原root节点现在需要新的左子节点了
	newRoot->right = preRoot;					//更改节点信息
	preRoot->left = RootL;						//原root节点的左孩子现在找到了新的节点
	//更新各点高度(newNode节点的左子树的各个节点高度一定不变,node节点的高度一定改变(但其右子节点高度不变))
	preRoot->height = max(GetNodeHeight(preRoot->left), GetNodeHeight(preRoot->right)) + 1;
	newRoot->height = max(GetNodeHeight(newRoot->left), GetNodeHeight(newRoot->right)) + 1;
	return newRoot;
}

//双左旋(R-L双旋转,RL插入(和单旋转不同))
template<class T>
TreeNode<T>* AVL<T>::DoubleRotateWithLeft(TreeNode<T>* preRoot)
{
	//先对preRoot->right进行右旋,转换为RR情况.再对preRoot进行左旋操作
	preRoot->right = SingleRotateWithRight(preRoot->right);
	//左旋单操作
	return SingleRotateWithLeft(preRoot);
}

//双右旋(L-R双旋转,LR插入(和单旋转不同))
template<class T>
TreeNode<T>* AVL<T>::DoubleRotateWithRight(TreeNode<T>* preRoot)
{
	//先对preRoot->left进行左旋,转换为LL情况.再对preRoot进行右旋操作
	preRoot->left = SingleRotateWithLeft(preRoot->left);
	//右旋单操作
	return SingleRotateWithRight(preRoot);
}

//插入节点的辅助函数
template<class T>
TreeNode<T>* AVL<T>::InsertNode(TreeNode<T>* node, const T& val)
{
	if (node == nullptr)
	{
		node = new TreeNode<T>(val);//height默认为0
		this->NodeSize++;
	}
	else
	{
		if (node->val >= val)
		{
			node->left = InsertNode(node->left, val);
			//查看是否需要旋转
			//L插入,可能导致RL插入或者LL插入(L插入一定有node的左子树高度大于node右子树高度且需要旋转时差为2)
			if (GetNodeHeight(node->left) - GetNodeHeight(node->right) == 2)
			{
				//此时一定需要旋转,LL插入是左旋
				//RL插入是
				if (val < node->left->val)
				{
					node = SingleRotateWithRight(node);
				}
				else
				{
					node = DoubleRotateWithRight(node);
				}
			}
		}
		else
		{
			node->right = InsertNode(node->right, val);
			//查看是否需要旋转
			//R插入,可能导致LR插入或者RR插入(R插入一定有node的右子树高度大于node左子树高度且需要旋转时差为2)
			if (GetNodeHeight(node->right) - GetNodeHeight(node->left) == 2)
			{
				//此时一定需要旋转,左旋！！！左旋！！！
				if (val > node->right->val)
				{
					node = SingleRotateWithLeft(node);
				}
				else
				{
					node = DoubleRotateWithLeft(node);
				}
			}
		}
	}

	//更新节点高度
	node->height = max(GetNodeHeight(node->left), GetNodeHeight(node->right)) + 1;
	return node;
}

//删除节点的辅助函数
template<class T>
TreeNode<T>* AVL<T>::DeleteNode(TreeNode<T>* node, const T& val)
{
	//旋转操作会自行维护节点的高度
	if (node == nullptr) { return node; }

	if (node->val > val)
	{
		//走左边
		node->left = DeleteNode(node->left, val);
		//判断是否失衡,删除了左子树的一个节点,因此判断右子树是否过高
		if (GetNodeHeight(node->right) - GetNodeHeight(node->left) > 1)
		{
			//右子树过高,则相当于向右子树中插入了节点
			if (GetNodeHeight(node->right->left) > GetNodeHeight(node->right->right))
			{
				//相当于RL插入
				node = DoubleRotateWithLeft(node);
			}
			else
			{
				//RR插入
				node = SingleRotateWithLeft(node);
			}
		}
	}
	else if (node->val < val)
	{
		//走右边
		node->right = DeleteNode(node->right, val);
		//判断是否失衡,删除了右子树的一个节点,因此判断左子树是否过高
		if (GetNodeHeight(node->left) - GetNodeHeight(node->right) > 1)
		{
			//左子树过高,则相当于向左子树中插入了节点
			if (GetNodeHeight(node->left->right) > GetNodeHeight(node->left->left))
			{
				//相当于LR插入
				node = DoubleRotateWithRight(node);
			}
			else
			{
				//LL插入
				node = SingleRotateWithRight(node);
			}
		}
	}
	else
	{
		//标准BST操作
		//找到节点了,当node->val == key 时
		//case 1: node为叶子节点,这种情况下直接将node删除即可
		//case 2: node节点只有左子树/右子树(这种情况可以包括case 1)
		//case 3: node节点既有左子树又有右子树(利用前驱节点来修改)
		if (node->left == nullptr)
		{
			//只有右子树
			TreeNode<T>* tempNode = node;//保存node指针
			node = node->right;			 //直接将node换成它的子节点
			delete tempNode;			 //释放原node节点的空间
			this->NodeSize--;			 //节点数减小
		}
		else if (node->right == nullptr)
		{
			//只有左子树
			TreeNode<T>* tempNode = node;//保存node指针
			node = node->left;			 //直接将node换成它的子节点
			delete tempNode;			 //释放原node节点的空间
			this->NodeSize--;			 //节点数减小
		}
		else
		{
			//case 3,左右子树均存在
			//如果node的左子树比右子树高则从左子树中选取
			if (GetNodeHeight(node->left) > GetNodeHeight(node->right))
			{
				//找到前驱节点
				TreeNode<T>* preNode = BST<T>::FindMaxNode(node->left);
				//找到前驱节点之后,交换preNode和node的值
				node->val = preNode->val;
				//接下来继续递归,从node的左子树出发,删除tempNode->val值的节点
				//(此时要删除的节点一定属于case 1或case 2)
				node->left = DeleteNode(node->left, preNode->val);
			}
			else
			{
				//找到next的后继节点
				TreeNode<T>* nextNode = BST<T>::FindMinNode(node->right);
				//交换node和nextNode的值
				node->val = nextNode->val;
				node->right = DeleteNode(node->right, nextNode->val);
			}
		}
	}
	return node;
}

//插入节点的函数
template<class T>
void AVL<T>::Insert(const T& val)
{
	this->root = InsertNode(this->root, val);
}

//删除节点的函数
template<class T>
void AVL<T>::Delete(const T& val)
{
	this->root = DeleteNode(this->root, val);
}
