#pragma once
#ifndef RETREE_H
#define RBTREE_H
#include "BSTree.h"

//红黑树
//（1）每个节点或者是黑色，或者是红色。
//（2）根节点是黑色。
//（3）每个叶子节点（NIL）是黑色。[注意：这里叶子节点，是指为空(NIL或NULL)的叶子节点！]
//（4）如果一个节点是红色的，则它的子节点必须是黑色的。
//（5）从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。
template<class T>
class RBT :virtual public BST<T>
{
private:
	//颜色
	enum color { RED, BLACK };	//EAD为0,BLACK为1
	//声明使用间接基类的root,否则无法访问
	using Tree<T>::root;
	using Tree<T>::NodeSize;
	//左旋
	void LeftRotate(TreeNode<T>* x);
	//右旋
	void RightRotate(TreeNode<T>* x);
	//插入调整函数
	void InsertFixUp(TreeNode<T>* node);
	//删除调整函数
	void DeleteFixUp(TreeNode<T>* node);
	//删除节点
	void DeleteNode(TreeNode<T>* node);
public:
	//构造函数
	RBT() {}
	//析构函数
	~RBT() {}
	//插入节点的函数
	void Insert(const T& val);
	//删除节点的函数
	void Delete(const T& val);
};

#endif // RETREE_H

//左旋
template<class T>
void RBT<T>::LeftRotate(TreeNode<T>* x)
{
	/*示意图
	*		p							p
	*		|							|
	*		x							y
	*	   / \			------->	   / \
	*	  lx  y						  x  ry
	*        / \					 / \
	*       ly ry					lx ly
	*/
	TreeNode<T>* y = x->right;
	//1.将x的右子节点指向y的左子节点(ly),将y的左子节点的父节点更新为x
	x->right = y->left;
	if (y->left != nullptr)
	{
		y->left->parent = x;
	}

	//2.当x的父节点(不为空时),更新y的父节点为x的父节点，并将x的父节点指定子树(当前x的子树位置)指定为y
	y->parent = x->parent;

	if (x->parent == nullptr)
	{
		//如果x为根节点
		this->root = y;
	}
	else if (x->parent->left == x)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}

	//3.将x的父节点更新为y，将y的左子节点更新为x
	x->parent = y;
	y->left = x;
}

//右旋
template<class T>
void RBT<T>::RightRotate(TreeNode<T>* x)
{
	/*示意图
	*		p							p
	*		|							|
	*		x							y
	*	   / \			------->	   / \
	*	  y	 rx						  ly  x
	*	 / \							 / \
	*   ly ry							ry rx
	*/
	TreeNode<T>* y = x->left;
	x->left = y->right;
	if (y->right != nullptr)
	{
		y->right->parent = x;
	}

	y->parent = x->parent;
	//将p左子树或者右子树的位置指向y(看x具体在那边)
	if (x->parent == nullptr)
	{
		this->root = y;
	}
	else if (x->parent->left == x)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}

	//将y的右子节点更新为x,x的父节点更新为y
	y->right = x;
	x->parent = y;
}

//插入调整函数
template<class T>
void RBT<T>::InsertFixUp(TreeNode<T>* node)
{
	//情景1:红黑树为空树，将跟节点染色为黑色（插入时已经处理,调整不需要处理）
	//情景2: 插入节点的父节点为黑色(不会破坏平衡,因此也不需要处理)
	//情景3: 插入节点的父节点为红色(此时该树的height一定大于等于3)
	//---- 情景3.1 : 叔叔节点存在，并且为红色(父 - 叔双红),将父节点和叔叔节点染为黑色,将爷爷节点染为红色,然后再进行递归处理(唯一一种会增加层数的情况)
	//---- 情景3.2:叔叔节点不存在，或者为黑色，父节点为爷爷节点的左子树
	//		--— 情景3.2.1 : 插入节点为其父节点的左子节点(LL情况),(父节点染黑,爷爷节点染红,并对爷爷节点右旋)
	//		---- 情景3.2.2 : 插入节点为其父节点的右子节点(LR情况),(先对父节点左旋,转换为LL情况)
	//---- 情景3.3 : 叔叔节点不存在，或者为黑色，父节点为爷爷节点的右子树
	//		—-- 情景3.3.1 : 插入节点为其父节点的右子节点(RR情况),(父节点染黑,爷爷节点染红,并对爷爷节点左旋)
	//		---- 情景3.3.2 : 插入节点为其父节点的左子节点(RL情况),(先对父节点右旋,转换为RR情况)
	this->root->color = BLACK;
	if (node->parent == nullptr || node->parent->color == BLACK) { return; }
	//找到叔叔节点
	TreeNode<T>* Father = node->parent;
	TreeNode<T>* GrandFather = Father->parent;
	int LRFlag = GrandFather->left == Father ? 1 : -1;//父节点为左子树则LRFlag = 1,否则LRFlag = -1
	TreeNode<T>* Uncle = LRFlag == 1 ? GrandFather->right : GrandFather->left;

	if (Uncle != nullptr && Uncle->color == RED)
	{
		//情景3.1
		//先染色
		Uncle->color = BLACK;
		Father->color = BLACK;
		GrandFather->color = RED;
		//递归
		InsertFixUp(GrandFather);
	}
	else if (Uncle == nullptr || Uncle->color == BLACK)
	{
		//情况3.2或3.3
		if (LRFlag == 1)
		{
			//父节点为左子树
			if (node == Father->right)
			{
				//LR情况,先对父节点左旋
				LeftRotate(Father);
				//此时的Father和node已经改变
				//交换Father和node
				swap(Father, node);
			}
			//情况3.2.1(LL情况)
			Father->color = BLACK;
			GrandFather->color = RED;
			RightRotate(GrandFather);
		}
		else
		{
			//父节点为右子树
			if (node == Father->left)
			{
				//RL情况,先对父节点右旋
				RightRotate(Father);
				//此时的Father和node已经改变
				//交换Father和node
				swap(Father, node);
			}
			//情况3.3.1,RR情况
			Father->color = BLACK;
			GrandFather->color = RED;
			LeftRotate(GrandFather);
		}
	}
}

//删除调整函数
template<class T>
void RBT<T>::DeleteFixUp(TreeNode<T>* node)
{
	while (node != this->root && node->color == BLACK) //当结点node不为根并且它的颜色不是黑色
	{
		if (node == node->parent->left)        //node在左子树
		{
			TreeNode<T>* brother = node->parent->right;    //brother是node的兄结点

			if (brother->color == RED)    //情况1
			{
				brother->color = BLACK;
				node->parent->color = RED;
				LeftRotate(node->parent);
				brother = node->parent->right;
			}

			if (brother->left->color == BLACK && brother->right->color == BLACK)    //情况2
			{
				brother->color = RED;
				node = node->parent;
			}
			else
			{
				if (brother->right->color == BLACK)        //情况3
				{
					brother->color = RED;
					brother->left->color = BLACK;
					RightRotate(brother);
					brother = node->parent->right;
				}
				//情况4
				brother->color = node->parent->color;
				node->parent->color = BLACK;
				brother->right->color = BLACK;
				LeftRotate(node->parent);
				node = this->root;    //结束循环
			}
		}
		else
		{
			//node在右子树
			TreeNode<T>* brother = node->parent->left;
			if (brother->color == RED)    //情况1
			{
				brother->color = BLACK;
				node->parent->color = RED;
				RightRotate(node->parent);
				brother = node->parent->left;
			}
			if (brother->left->color == BLACK && brother->right->color == BLACK)        //情况2
			{
				brother->color = RED;
				node = node->parent;
			}
			else
			{
				if (brother->left->color == BLACK)    //情况3
				{
					brother->color = RED;
					brother->right->color = BLACK;
					LeftRotate(brother);
					brother = node->parent->left;
				}
				//情况4
				brother->color = node->parent->color;
				node->parent->color = BLACK;
				brother->left->color = BLACK;
				RightRotate(node->parent);
				node = this->root;    //结束循环
			}
		}
	}
	node->color = BLACK;
}

//删除辅助函数
template<class T>
void RBT<T>::DeleteNode(TreeNode<T>* node)
{
	if (node->left != nullptr && node->right != nullptr)
	{
		//node有双子树
		//找到后继节点
		TreeNode<T>* nextNode = this->FindMinNode(node->right);
		node->val = nextNode->val;		//将node节点的值改为其后继节点的值,则后续只需删除后继节点
		node = nextNode;				//node指针现在指向原node节点的后继节点,然后准备平衡
	}

	//观察node节点是否有左右子树(此时的node节点的孩子一定不为双子树),node节点也可能是叶子节点
	TreeNode<T>* replacement = node->left != nullptr ? node->left : node->right;

	if (replacement != nullptr)
	{
		// Link replacement to parent
		replacement->parent = node->parent;
		//node->parent要找到replacement做其儿子(将replacement代替掉node)
		if (node->parent == nullptr)
			this->root = replacement;
		else if (node == node->parent->left)
			node->parent->left = replacement;		//node节点原来是其父节点的左儿子
		else
			node->parent->right = replacement;		//node节点原来是其父节点的右儿子

		// 如果删除的是个黑色节点,则需要调整平衡,否则直接删除即可
		if (node->color == BLACK)
			DeleteFixUp(replacement);
	}
	else if (node->parent == nullptr)
	{
		//如果要删除的节点node为叶子节点,则删除掉node节点后一定为root空
		this->root = nullptr;
	}
	else
	{
		//如果要删除的node节点为叶子节点(如果是红色节点,直接删除即可,但黑色节点需要旋转调整)
		if (node->color == BLACK)
			DeleteFixUp(node);//以node节点为支点调整红黑树平衡
		//调整后的node节点的父节点不为nullptr(或者一开始node->color = RED,一定要避免野指针)
		if (node->parent != nullptr)
		{
			//指针清空(避免野指针)
			if (node == node->parent->left)
				node->parent->left = nullptr;
			else if (node == node->parent->right)
				node->parent->right = nullptr;
			node->parent = nullptr;
		}
	}
	//回收node节点
	delete node;
}

//插入节点的函数
template<class T>
void RBT<T>::Insert(const T& val)
{
	//新节点的color为RED(0)
	TreeNode<T>* node = new TreeNode<T>(val);
	//插入后可能破坏红黑树颜色平衡
	if (this->root == nullptr)
	{
		node->color = BLACK;
		this->root = node;
	}
	else
	{
		TreeNode<T>* parent = nullptr;	//node可能的父节点
		TreeNode<T>* curNode = this->root;
		while (curNode != nullptr)
		{
			parent = curNode;
			//查找node可插入位置
			if (curNode->val >= node->val)
			{
				//左走
				curNode = curNode->left;
			}
			else
			{
				//右走
				curNode = curNode->right;
			}
		}
		//设置父节点(退出时curNode为空,但parent一定不为空)
		node->parent = parent;
		//将父节点的左右孩子设置为node
		if (parent->val >= node->val)
		{
			parent->left = node;
		}
		else
		{
			parent->right = node;
		}
		//调整平衡
		InsertFixUp(node);
	}
}

//删除节点的函数
template<class T>
void RBT<T>::Delete(const T& val)
{
	//删除操作,参考BST标准删除操作,由于可能破坏RBT的平衡状态,因此需要重新平衡
	//设要删除的节点为x,则找到其前驱或者后继节点p,p一定为叶子节点或者只有一颗子树
	//替换x的值,则真正删除的节点为p,若p为红色节点,则可直接删除,若p为黑色节点,则不可直接删除,否则会导致黑色失衡
	TreeNode<T>* delNode = this->GetNode(val);
	if (delNode != nullptr) { DeleteNode(delNode); }
}
