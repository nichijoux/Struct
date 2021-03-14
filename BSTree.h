#pragma once
#ifndef _BSTTREE_H
#define _BSTTREE_H
#include "Tree.h"

template<class T, class compare = Less<T>>
class BST :virtual public Tree<T, compare>
{
protected:
	//将树清空
	void ClearTree(TreeNode<T>* node);
	//插入节点的辅助函数(递归实现)
	virtual TreeNode<T>* InsertNode(TreeNode<T>* node, const T& val);
	//删除节点的辅助函数
	virtual TreeNode<T>* DeleteNode(TreeNode<T>* node, const T& val);
	//找到以node节点为根节点的的最小值节点
	TreeNode<T>* FindMinNode(TreeNode<T>* node)const;
	//找到以node节点为根节点的的最大值节点
	TreeNode<T>* FindMaxNode(TreeNode<T>* node)const;
public:
	//构造函数(会自动调用父类的构造函数)
	BST() {}
	//析构函数
	virtual ~BST() { ClearTree(this->root); }

	//插入节点的函数
	virtual void Insert(const T& val);
	//删除节点的函数
	virtual void Delete(const T& val);
	//判断BST中是否存在val
	bool Search(const T& val);
	//得到BST中指定值的节点
	TreeNode<T>* GetNode(const T& val);
	//返回BST中最小值的节点
	TreeNode<T>* GetMinNode();
	//返回BST中最大值的节点
	TreeNode<T>* GetMaxNode();
};
#endif // !BSTtree.h


//函数的实现,分离头文件和源文件需要BSTree.h和BSTree.cpp 使用时必须#include "BSTree.cpp"不然编译器找不到文件
//但是.cpp太难看了,去掉.cpp使得BSTree.cpp变为BSTree文件(就像iostream一样)也可以编译使用,但感觉还是挺难看的
//所以还是直接全都写在一起算了,定义在前,实现在后

//将树清空,inline 内联函数
template<class T, class compare>
inline void BST<T, compare>::ClearTree(TreeNode<T>* node)
{
	if (node != nullptr)
	{
		ClearTree(node->left);
		ClearTree(node->right);
		delete node;
	}
}

//插入节点的辅助函数(递归实现)
template <class T, class compare>
TreeNode<T>* BST<T, compare>::InsertNode(TreeNode<T>* node, const T& val)
{
	if (node == nullptr)
	{
		node = new TreeNode<T>(val);
		this->NodeSize++;
	}
	else
	{
		if (node->val >= val)
		{
			node->left = InsertNode(node->left, val);
		}
		else
		{
			node->right = InsertNode(node->right, val);
		}
	}
	return node;
}

//删除节点的辅助函数(递归实现)
template<class T, class compare>
TreeNode<T>* BST<T, compare>::DeleteNode(TreeNode<T>* node, const T& key)
{
	//递归终止条件
	if (node == nullptr) { return node; }

	if (node->val > key)
	{
		//走左边
		node->left = DeleteNode(node->left, key);
	}
	else if (node->val < key)
	{
		//走右边
		node->right = DeleteNode(node->right, key);
	}
	else
	{
		//当node->val == key 时
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
			//找到node的前驱节点
			//由于BST树的中序遍历为有序上升数组
			//因此对node的前驱一定为中序遍历中排在node前面的那个数
			//因此node的前驱pre一定为叶子节点或者只有左子树的节点（因为node是第一个大于pre的节点）
			srand(time(NULL));
			int randNum = rand() % 100;
			//为了避免一直删除前驱节点造成的不平衡影响,使用rand使得随机删除前驱或者后继
			if ((randNum & 1) == 0)
			{
				//找到前驱节点
				TreeNode<T>* preNode = FindMaxNode(node->left);
				//找到前驱节点之后,交换preNode和node的值
				node->val = preNode->val;
				//接下来继续递归,从node的左子树出发,删除tempNode->val值的节点
				//(此时要删除的节点一定属于case 1或case 2)
				node->left = DeleteNode(node->left, preNode->val);
			}
			else
			{
				//找到next的后继节点
				TreeNode<T>* nextNode = FindMinNode(node->right);
				//交换node和nextNode的值
				node->val = nextNode->val;
				node->right = DeleteNode(node->right, nextNode->val);
			}
		}
	}
	//删除后记得返回
	return node;
}

//返回BST中最小值的节点
template<class T, class compare>
TreeNode<T>* BST<T, compare>::FindMinNode(TreeNode<T>* node)const
{
	TreeNode<T>* tempNode = node;
	if (tempNode != nullptr)
	{
		while (tempNode->left != nullptr)
		{
			tempNode = tempNode->left;
		}
	}
	return tempNode;
}

//返回BST中最大值的节点
template<class T, class compare>
TreeNode<T>* BST<T, compare>::FindMaxNode(TreeNode<T>* node)const
{
	TreeNode<T>* tempNode = node;
	if (tempNode != nullptr)
	{
		while (tempNode->right != nullptr)
		{
			tempNode = tempNode->right;
		}
	}
	return tempNode;
}

//插入函数的实现
template<class T, class compare>
void BST<T, compare>::Insert(const T& val)
{
	//迭代实现
	if (this->root == nullptr)
	{
		this->root = new TreeNode<T>(val);
		this->NodeSize++;
	}
	else
	{
		//否则寻找合适的位置插入
		TreeNode<T>* tempNode = this->root;
		while (tempNode != nullptr)
		{
			if (tempNode->val >= val)
			{
				//说明应该向左走
				if (tempNode->left == nullptr)
				{
					tempNode->left = new TreeNode<T>(val);
					this->NodeSize++;
					return;
				}
				tempNode = tempNode->left;
			}
			else
			{
				//否则走右边
				if (tempNode->right == nullptr)
				{
					tempNode->right = new TreeNode<T>(val);
					this->NodeSize++;
					return;
				}
				tempNode = tempNode->right;
			}
		}
	}

	//递归实现(调用递归函数)
	//this->root = InsertNode(this->root, val);
}

//删除值为val的节点
template<class T, class compare>
void BST<T, compare>::Delete(const T& val)
{
	if (this->NodeSize <= 0) { return; }
	//删除一个值为val的节点
	this->root = DeleteNode(this->root, val);
}

//判断BST中是否存在val
template<class T, class compare>
bool BST<T, compare>::Search(const T& val)
{
	TreeNode<T>* tempNode = this->root;
	while (tempNode != nullptr)
	{
		if (tempNode->val == val)
		{
			return true;
		}
		else if (tempNode->val > val)
		{
			//val可能在tempNode的左边
			tempNode = tempNode->left;
		}
		else
		{
			//val可能在tempNode的右边
			tempNode = tempNode->right;
		}
	}
	//表示未找到
	return false;
}

//得到BST中指定值的节点
template<class T, class compare>
TreeNode<T>* BST<T, compare>::GetNode(const T& val)
{
	TreeNode<T>* tempNode = this->root;
	while (tempNode != nullptr && tempNode->val != val)
	{
		if (tempNode->val >= val)
		{
			//向左找
			tempNode = tempNode->left;
		}
		else
		{
			tempNode = tempNode->right;
		}
	}
	return tempNode;
}

//返回BST中最小值的节点
template<class T, class compare>
TreeNode<T>* BST<T, compare>::GetMinNode()
{
	return FindMinNode(this->root);
}

//返回BST中最大值的节点
template<class T, class compare>
TreeNode<T>* BST<T, compare>::GetMaxNode()
{
	return FindMaxNode(this->root);
}
