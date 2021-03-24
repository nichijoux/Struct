#pragma once
#ifndef RETREE_H
#define RBTREE_H
using std::max;
using std::swap;

template<class K, class V>
struct RBTNode
{
	K key;					//键
	V val;					//值
	int color;				//颜色(0为RED,1为BLACK)
	RBTNode<K, V>* left;
	RBTNode<K, V>* right;
	RBTNode<K, V>* parent;
	RBTNode() = default;
	RBTNode(K key, V val) : key(key), val(val), color(0), left(nullptr), right(nullptr), parent(nullptr) {}
};

//红黑树
//（1）每个节点或者是黑色，或者是红色。
//（2）根节点是黑色。
//（3）每个叶子节点（NIL）是黑色。[注意：这里叶子节点，是指为空(NIL或NULL)的叶子节点！]
//（4）如果一个节点是红色的，则它的子节点必须是黑色的。
//（5）从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。
template<class K, class V>
class RBT
{
private:
	//颜色
	enum color { RED, BLACK };	//RAD为0,BLACK为1
	RBTNode<K, V>* root;		//树的根节点
	int NodeSize;				//树节点总数

	//将树清空
	void ClearTree(RBTNode<K, V>* node);
	//返回RBT中以node节点为根节点的最小key节点
	RBTNode<K, V>* FindMinNode(RBTNode<K, V>* node);
	//返回RBT中以node节点为根节点的最大key节点
	RBTNode<K, V>* FindMaxNode(RBTNode<K, V>* node);

	//左旋
	void LeftRotate(RBTNode<K, V>* x);
	//右旋
	void RightRotate(RBTNode<K, V>* x);
	//插入调整函数
	void InsertFixUp(RBTNode<K, V>* node);
	//删除调整函数
	void DeleteFixUp(RBTNode<K, V>* node);
	//删除节点
	void DeleteNode(RBTNode<K, V>* node);

	//前序遍历的辅助函数(RBTNode* 可更改node的值)
	void preOrderHelp(RBTNode<K, V>* node, void(*function)(RBTNode<K, V>* node));
	//中序遍历的辅助函数(RBTNode* 可更改node的值)
	void inOrderHelp(RBTNode<K, V>* node, void(*function)(RBTNode<K, V>* node));
	//后序遍历的辅助函数(RBTNode* 可更改node的值)
	void backOrderHelp(RBTNode<K, V>* node, void(*function)(RBTNode<K, V>* node));

	//node节点的颜色
	int colorOf(RBTNode<K, V>* node);
	//node节点的左子节点
	RBTNode<K, V>* leftOf(RBTNode<K, V>* node);
	//node节点的右子节点
	RBTNode<K, V>* rightOf(RBTNode<K, V>* node);
	//node节点的父节点
	RBTNode<K, V>* parentOf(RBTNode<K, V>* node);
	//设置node节点的颜色
	void setColor(RBTNode<K, V>* node, int color);
	//得到树的高度的辅助函数
	int get_Height_Help(RBTNode<K, V>* node)const;
public:
	//构造函数
	RBT() { root = nullptr; NodeSize = 0; }
	//析构函数
	~RBT() { Clear(); }
	//插入节点的函数
	void Insert(const K& key, const V& val);
	//删除节点的函数
	void Delete(const K& key);
	//判断RBT中是否存在键值为key的节点
	bool Search(const K& key)const;
	//RBT树清空
	void Clear() { ClearTree(this->root); this->root = nullptr; this->NodeSize = 0; }
	//得到RBT中指定key值节点
	RBTNode<K, V>* GetNode(const K& key)const;
	//返回RBT中最小值的节点
	RBTNode<K, V>* GetMinNode()const;
	//返回RBT中最大值的节点
	RBTNode<K, V>* GetMaxNode()const;
	//得到RBT树的高度
	int GetHeight()const { return get_Height_Help(this->root); }
	//先序遍历
	void preOrder(void(*function)(RBTNode<K, V>* node)) { preOrderHelp(this->root, function); }
	//中序遍历
	void inOrder(void(*function)(RBTNode<K, V>* node)) { inOrderHelp(this->root, function); }
	//后序遍历
	void backOrder(void(*function)(RBTNode<K, V>* node)) { backOrderHelp(this->root, function); }

	//重载[]操作符
	V& operator[](const K& key);

	//防止拷贝构造
	RBT(const RBT<K, V>& anotherTree) = delete;
	RBT<K, V>& operator=(const RBT<K, V>& anotherTree) = delete;
};


//将树清空
template<class K, class V>
void RBT<K, V>::ClearTree(RBTNode<K, V>* node)
{
	if (node == nullptr) { return; }
	ClearTree(node->left);
	ClearTree(node->right);
	delete node;
}

//返回RBT中以node节点为根节点的最小key节点
template<class K, class V>
RBTNode<K, V>* RBT<K, V>::FindMinNode(RBTNode<K, V>* node)
{
	RBTNode<K, V>* tempNode = node;
	if (tempNode != nullptr)
	{
		while (tempNode->left != nullptr)
		{
			tempNode = tempNode->left;
		}
	}
	return tempNode;
}

//返回RBT中以node节点为根节点的最大key节点
template<class K, class V>
RBTNode<K, V>* RBT<K, V>::FindMaxNode(RBTNode<K, V>* node)
{
	RBTNode<K, V>* tempNode = node;
	if (tempNode != nullptr)
	{
		while (tempNode->right != nullptr)
		{
			tempNode = tempNode->right;
		}
	}
	return tempNode;
}

//node节点的颜色
template<class K, class V>
inline int RBT<K, V>::colorOf(RBTNode<K, V>* node)
{
	return node == nullptr ? BLACK : node->color;
}

//node节点的左子节点
template<class K, class V>
inline RBTNode<K, V>* RBT<K, V>::leftOf(RBTNode<K, V>* node)
{
	return node == nullptr ? nullptr : node->left;
}

//node节点的右子节点
template<class K, class V>
inline RBTNode<K, V>* RBT<K, V>::rightOf(RBTNode<K, V>* node)
{
	return node == nullptr ? nullptr : node->right;
}

//node节点的父节点
template<class K, class V>
inline RBTNode<K, V>* RBT<K, V>::parentOf(RBTNode<K, V>* node)
{
	return node == nullptr ? nullptr : node->parent;
}

//设置node节点的颜色
template<class K, class V>
inline void RBT<K, V>::setColor(RBTNode<K, V>* node, int color)
{
	if (node != nullptr)
		node->color = color;
}

//得到树的高度的辅助函数
template<class K, class V>
int RBT<K, V>::get_Height_Help(RBTNode<K, V>* node)const
{
	if (node == nullptr) { return 0; }
	return max(get_Height_Help(node->left), get_Height_Help(node->right)) + 1;
}

//左旋
template<class K, class V>
void RBT<K, V>::LeftRotate(RBTNode<K, V>* x)
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
	RBTNode<K, V>* y = x->right;
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
template<class K, class V>
void RBT<K, V>::RightRotate(RBTNode<K, V>* x)
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
	RBTNode<K, V>* y = x->left;
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
template<class K, class V>
void RBT<K, V>::InsertFixUp(RBTNode<K, V>* node)
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
	setColor(this->root, BLACK);
	if (parentOf(node) == nullptr || colorOf(parentOf(node)) == BLACK) { return; }
	//找到叔叔节点
	RBTNode<K, V>* Father = parentOf(node);
	RBTNode<K, V>* GrandFather = parentOf(Father);
	int LRFlag = leftOf(GrandFather) == Father ? 1 : -1;//父节点为左子树则LRFlag = 1,否则LRFlag = -1
	RBTNode<K, V>* Uncle = LRFlag == 1 ? rightOf(GrandFather) : leftOf(GrandFather);

	if (Uncle != nullptr && Uncle->color == RED)
	{
		//情景3.1
		//先染色
		setColor(Uncle, BLACK);
		setColor(Father, BLACK);
		setColor(GrandFather, RED);
		//递归
		InsertFixUp(GrandFather);
	}
	else if (Uncle == nullptr || colorOf(Uncle) == BLACK)
	{
		//情况3.2或3.3
		if (LRFlag == 1)
		{
			//父节点为左子树
			if (node == rightOf(Father))
			{
				//LR情况,先对父节点左旋
				LeftRotate(Father);
				//此时的Father和node已经改变
				//交换Father和node
				swap(Father, node);
			}
			//情况3.2.1(LL情况)
			setColor(Father, BLACK);
			setColor(GrandFather, RED);
			RightRotate(GrandFather);
		}
		else
		{
			//父节点为右子树
			if (node == leftOf(Father))
			{
				//RL情况,先对父节点右旋
				RightRotate(Father);
				//此时的Father和node已经改变
				//交换Father和node
				swap(Father, node);
			}
			//情况3.3.1,RR情况
			setColor(Father, BLACK);
			setColor(GrandFather, RED);
			LeftRotate(GrandFather);
		}
	}
}

//删除调整函数
template<class K, class V>
void RBT<K, V>::DeleteFixUp(RBTNode<K, V>* node)
{
	while (node != this->root && colorOf(node) == BLACK) //当结点node不为根并且它的颜色不是黑色
	{
		if (node == leftOf(parentOf(node)))
		{
			//node在左子树
			RBTNode<K, V> brother = rightOf(parentOf(node));    //brother节点是node节点的兄弟结点
			if (colorOf(brother) == RED)    //情况1
			{
				setColor(brother, BLACK);
				setColor(parentOf(node), RED);
				LeftRotate(parentOf(node));
				brother = rightOf(parentOf(node));
			}

			if (colorOf(leftOf(brother)) == BLACK && colorOf(rightOf(brother)) == BLACK)    //情况2
			{
				setColor(brother, RED);
				node = parentOf(node);
			}
			else
			{
				if (colorOf(rightOf(brother)) == BLACK)        //情况3
				{
					setColor(brother, RED);
					setColor(leftOf(brother), BLACK);
					RightRotate(brother);
					brother = rightOf(parentOf(node));
				}
				//情况4
				setColor(brother, colorOf(parentOf(node)));
				setColor(parentOf(node), BLACK);
				setColor(rightOf(brother), BLACK);
				LeftRotate(parentOf(node));
				node = this.root;    //结束循环
			}
		}
		else
		{
			//node在右子树
			RBTNode<K, V> brother = leftOf(parentOf(node));		//brother节点为node节点的兄弟节点
			if (colorOf(brother) == RED)    //情况1
			{
				setColor(brother, BLACK);
				setColor(parentOf(node), RED);
				RightRotate(parentOf(node));
				brother = rightOf(parentOf(node));
			}
			if (colorOf(leftOf(brother)) == BLACK && colorOf(rightOf(brother)) == BLACK)        //情况2
			{
				setColor(brother, RED);
				node = parentOf(node);
			}
			else
			{
				if (colorOf(leftOf(brother)) == BLACK)    //情况3
				{
					setColor(brother, RED);
					setColor(rightOf(brother), BLACK);
					LeftRotate(brother);
					brother = leftOf(parentOf(node));
				}
				//情况4
				setColor(brother, colorOf(parentOf(node)));
				setColor(parentOf(node), BLACK);
				setColor(leftOf(brother), BLACK);
				RightRotate(parentOf(node));
				node = this->root;    //结束循环
			}
		}
	}
	node->color = BLACK;
}

//删除辅助函数
template<class K, class V>
void RBT<K, V>::DeleteNode(RBTNode<K, V>* node)
{
	if (node->left != nullptr && node->right != nullptr)
	{
		//node有双子树
		//找到后继节点
		RBTNode<K, V>* nextNode = this->FindMinNode(node->right);
		node->val = nextNode->val;		//将node节点的值改为其后继节点的值,则后续只需删除后继节点
		node = nextNode;				//node指针现在指向原node节点的后继节点,然后准备平衡
	}

	//观察node节点是否有左右子树(此时的node节点的孩子一定不为双子树),node节点也可能是叶子节点
	RBTNode<K, V>* replacement = node->left != nullptr ? node->left : node->right;

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
	delete node;
}

//前序遍历的辅助函数(RBTNode* 可更改node的值)
template<class K, class V>
void RBT<K, V>::preOrderHelp(RBTNode<K, V>* node, void(*function)(RBTNode<K, V>* node))
{
	if (node == nullptr) { return; }
	function(node);
	preOrderHelp(node->left, function);
	preOrderHelp(node->right, function);
}

//中序遍历的辅助函数(RBTNode* 可更改node的值)
template<class K, class V>
void RBT<K, V>::inOrderHelp(RBTNode<K, V>* node, void(*function)(RBTNode<K, V>* node))
{
	if (node == nullptr) { return; }
	inOrderHelp(node->left, function);
	function(node);
	inOrderHelp(node->right, function);
}

//后序遍历的辅助函数(RBTNode* 可更改node的值)
template<class K, class V>
void RBT<K, V>::backOrderHelp(RBTNode<K, V>* node, void(*function)(RBTNode<K, V>* node))
{
	if (node == nullptr) { return; }
	backOrderHelp(node->left, function);
	backOrderHelp(node->right, function);
	function(node);
}

//插入节点的函数
template<class K, class V>
void RBT<K, V>::Insert(const K& key, const V& val)
{
	//新节点的color为RED(0)
	RBTNode<K, V>* node = new RBTNode<K, V>(key, val);
	//插入后可能破坏红黑树颜色平衡
	if (this->root == nullptr)
	{
		node->color = BLACK;
		this->root = node;
	}
	else
	{
		RBTNode<K, V>* parent = nullptr;	//node可能的父节点
		RBTNode<K, V>* curNode = this->root;
		while (curNode != nullptr)
		{
			parent = curNode;
			//查找node可插入位置
			if (curNode->key > node->key)
			{
				//左走
				curNode = curNode->left;
			}
			else if (curNode->key < node->key)
			{
				//右走
				curNode = curNode->right;
			}
			else
			{
				//节点值相同
				curNode->val = val;
				return;
			}
		}
		//设置父节点(退出时curNode为空,但parent一定不为空)
		node->parent = parent;
		//将父节点的左右孩子设置为node
		if (parent->key > node->key)
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
	this->NodeSize++;
}

//删除节点的函数
template<class K, class V>
void RBT<K, V>::Delete(const K& key)
{
	//删除操作,参考BST标准删除操作,由于可能破坏RBT的平衡状态,因此需要重新平衡
	//设要删除的节点为x,则找到其前驱或者后继节点p,p一定为叶子节点或者只有一颗子树
	//替换x的值,则真正删除的节点为p,若p为红色节点,则可直接删除,若p为黑色节点,则不可直接删除,否则会导致黑色失衡
	RBTNode<K, V>* delNode = this->GetNode(key);
	if (delNode != nullptr) { DeleteNode(delNode); this->NodeSize--; }
}

//判断RBT中是否存在键值为key的节点
template<class K, class V>
bool RBT<K, V>::Search(const K& key)const
{
	return GetNode(key) != nullptr;
}

//得到指定key值节点
template<class K, class V>
RBTNode<K, V>* RBT<K, V>::GetNode(const K& key)const
{
	RBTNode<K, V>* tempNode = this->root;
	while (tempNode != nullptr && tempNode->key != key)
	{
		if (tempNode->key > key)
		{
			//走左边
			tempNode = tempNode->left;
		}
		else if (tempNode->key < key)
		{
			tempNode = tempNode->right;
		}
	}
	return tempNode;
}

//返回RBT中最小值的节点
template<class K, class V>
inline RBTNode<K, V>* RBT<K, V>::GetMinNode()const
{
	return FindMinNode(this->root);
}

//返回RBT中最大值的节点
template<class K, class V>
inline RBTNode<K, V>* RBT<K, V>::GetMaxNode()const
{
	return FindMaxNode(this->root);
}

//重载[]操作符
template<class K, class V>
V& RBT<K, V>::operator[](const K& key)
{
	RBTNode<K, V>* tempNode = GetNode(key);
	if (tempNode == nullptr)
	{
		V val{};			//C++ 11
		Insert(key, val);
		tempNode = GetNode(key);
	}
	return tempNode->val;
}

#endif // RETREE_H
