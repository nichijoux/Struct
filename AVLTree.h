#pragma once
#ifndef AVLTREE_H
#define AVLTREE_H
using std::max;
using std::swap;

//AVL树的节点
template<class K, class V>
struct AVLNode
{
	K key;					//键
	V val;					//值
	int height;				//AVL树使用
	AVLNode<K, V>* left;
	AVLNode<K, V>* right;
	AVLNode() = default;
	AVLNode(K key, V val) : key(key), val(val), height(0), left(nullptr), right(nullptr) {}
};

//AVL树,AVL树是带平衡条件的BST
//AVL树是每个节点的左子树和右子树的高度最多差1的二叉查找树
template<class K, class V>
class AVL
{
private:
	AVLNode<K, V>* root;	//树的根节点
	int NodeSize;			//树节点总数
	//得到某一节点的高度
	int GetNodeHeight(AVLNode<K, V>* node)const;
	//将树清空
	void ClearTree(AVLNode<K, V>* node);

	//前序遍历的辅助函数(AVLNode* 可更改node的值)
	void preOrderHelp(AVLNode<K, V>* node, void(*function)(AVLNode<K, V>* node));
	//中序遍历的辅助函数(AVLNode* 可更改node的值)
	void inOrderHelp(AVLNode<K, V>* node, void(*function)(AVLNode<K, V>* node));
	//后序遍历的辅助函数(AVLNode* 可更改node的值)
	void backOrderHelp(AVLNode<K, V>* node, void(*function)(AVLNode<K, V>* node));

	//返回AVL中最小值的节点
	AVLNode<K, V>* FindMinNode(AVLNode<K, V>* node)const;
	//返回AVL中最大值的节点
	AVLNode<K, V>* FindMaxNode(AVLNode<K, V>* node)const;

	//插入节点的辅助函数
	AVLNode<K, V>* InsertNode(AVLNode<K, V>* node, const K& key, const V& val);
	//删除节点的辅助函数
	AVLNode<K, V>* DeleteNode(AVLNode<K, V>* node, const K& key);

	//转,转就完事儿
	//单左旋,插入时有RR插入,即向右子树的右孩子插入节点,导致不符合AVL树的定义
	AVLNode<K, V>* SingleRotateWithLeft(AVLNode<K, V>* preRoot);
	//单右旋,插入时有LL插入,即向左子树的左孩子插入节点,导致不符合AVL树的定义
	AVLNode<K, V>* SingleRotateWithRight(AVLNode<K, V>* preRoot);
	//双旋转实际是需要旋转两次
	//双左旋(R-L双旋转,RL插入(和单旋转不同))
	AVLNode<K, V>* DoubleRotateWithLeft(AVLNode<K, V>* preRoot);
	//双右旋(L-R双旋转,LR插入(和单旋转不同))
	AVLNode<K, V>* DoubleRotateWithRight(AVLNode<K, V>* preRoot);
public:
	//构造函数
	AVL() { root = nullptr; NodeSize = 0; }
	//析构函数
	~AVL() { Clear(); }
	//插入节点的函数
	void Insert(const K& key, const V& val);
	//删除节点的函数
	void Delete(const K& key);
	//判断AVL中是否存在val
	bool Search(const K& key)const;
	//AVL树清空
	void Clear() { ClearTree(this->root); root = nullptr; NodeSize = 0; }
	//得到树的高度
	int GetHeight() const { return root == nullptr ? 0 : root->height; }
	//得到AVL中指定值的节点
	AVLNode<K, V>* GetNode(const K& key)const;
	//返回AVL中最小值的节点
	AVLNode<K, V>* GetMinNode()const;
	//返回AVL中最大值的节点
	AVLNode<K, V>* GetMaxNode()const;
	//前序遍历
	void preOrder(void(*function)(AVLNode<K, V>* node)) { preOrderHelp(this->root, function); };
	//中序遍历
	void inOrder(void(*function)(AVLNode<K, V>* node)) { inOrderHelp(this->root, function); }
	//后序遍历
	void backOrder(void(*function)(AVLNode<K, V>* node)) { backOrderHelp(this->root, function); }

	//重载[]操作符
	V& operator[](const K& key);

	//防止拷贝构造
	AVL(const AVL<K, V>& anotherTree) = delete;
	AVL<K, V>& operator=(const AVL<K, V>& anotherTree) = delete;
};

//得到某节点的高度
template<class K, class V>
inline int AVL<K, V>::GetNodeHeight(AVLNode<K, V>* node)const
{
	return node == nullptr ? 0 : node->height;
}

//将树清空
template<class K, class V>
void AVL<K, V>::ClearTree(AVLNode<K, V>* node)
{
	if (node == nullptr) { return; }
	ClearTree(node->left);
	ClearTree(node->right);
	delete node;
}

//前序遍历的辅助函数(AVLNode* 可更改node的值)
template<class K, class V>
void AVL<K, V>::preOrderHelp(AVLNode<K, V>* node, void(*function)(AVLNode<K, V>* node))
{
	if (node == nullptr) { return; }
	function(node);
	preOrderHelp(node->left, function);
	preOrderHelp(node->right, function);
}

//中序遍历的辅助函数(AVLNode* 可更改node的值)
template<class K, class V>
void AVL<K, V>::inOrderHelp(AVLNode<K, V>* node, void(*function)(AVLNode<K, V>* node))
{
	if (node == nullptr) { return; }
	inOrderHelp(node->left, function);
	function(node);
	inOrderHelp(node->right, function);
}

//后序遍历的辅助函数(AVLNode* 可更改node的值)
template<class K, class V>
void AVL<K, V>::backOrderHelp(AVLNode<K, V>* node, void(*function)(AVLNode<K, V>* node))
{
	if (node == nullptr) { return; }
	backOrderHelp(node->left, function);
	backOrderHelp(node->right, function);
	function(node);
}

//返回AVL中最小值的节点
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::FindMinNode(AVLNode<K, V>* node)const
{
	AVLNode<K, V>* tempNode = node;
	if (tempNode != nullptr)
	{
		while (tempNode->left != nullptr)
		{
			tempNode = tempNode->left;
		}
	}
	return tempNode;
}

//返回AVL中最大值的节点
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::FindMaxNode(AVLNode<K, V>* node)const
{
	AVLNode<K, V>* tempNode = node;
	if (tempNode != nullptr)
	{
		while (tempNode->right != nullptr)
		{
			tempNode = tempNode->right;
		}
	}
	return tempNode;
}

//旋转吧！雪月花
//单左旋,插入时有RR插入,即向右子树的右孩子插入节点,导致不符合AVL树的定义
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::SingleRotateWithLeft(AVLNode<K, V>* preRoot)
{
	//RR插入使得preRoot变为第一个不满足AVL树定义的节点
	AVLNode<K, V>* newRoot = preRoot->right;		//将newRoot节点作为新的root节点
	AVLNode<K, V>* RootR = newRoot->left;			//preRoot->right应该更新为RootR
	newRoot->left = preRoot;					//逆袭了
	preRoot->right = RootR;						//原root节点的右孩子现在找到了新的节点
	//更新各点高度
	preRoot->height = max(GetNodeHeight(preRoot->left), GetNodeHeight(preRoot->right)) + 1;
	newRoot->height = max(GetNodeHeight(newRoot->left), GetNodeHeight(newRoot->right)) + 1;
	return newRoot;
}

//单右旋,插入时有LL插入,即向左子树的左孩子插入节点,导致不符合AVL树的定义
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::SingleRotateWithRight(AVLNode<K, V>* preRoot)
{
	//LL插入使得preRoot变为第一个不满足AVL树定义的节点
	AVLNode<K, V>* newRoot = preRoot->left;		//将newRoot节点作为新的root节点
	AVLNode<K, V>* RootL = newRoot->right;		//原root节点现在需要新的左子节点了
	newRoot->right = preRoot;					//更改节点信息
	preRoot->left = RootL;						//原root节点的左孩子现在找到了新的节点
	//更新各点高度(newNode节点的左子树的各个节点高度一定不变,node节点的高度一定改变(但其右子节点高度不变))
	preRoot->height = max(GetNodeHeight(preRoot->left), GetNodeHeight(preRoot->right)) + 1;
	newRoot->height = max(GetNodeHeight(newRoot->left), GetNodeHeight(newRoot->right)) + 1;
	return newRoot;
}

//双左旋(R-L双旋转,RL插入(和单旋转不同))
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::DoubleRotateWithLeft(AVLNode<K, V>* preRoot)
{
	//先对preRoot->right进行右旋,转换为RR情况.再对preRoot进行左旋操作
	preRoot->right = SingleRotateWithRight(preRoot->right);
	//左旋单操作
	return SingleRotateWithLeft(preRoot);
}

//双右旋(L-R双旋转,LR插入(和单旋转不同))
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::DoubleRotateWithRight(AVLNode<K, V>* preRoot)
{
	//先对preRoot->left进行左旋,转换为LL情况.再对preRoot进行右旋操作
	preRoot->left = SingleRotateWithLeft(preRoot->left);
	//右旋单操作
	return SingleRotateWithRight(preRoot);
}

//插入节点的辅助函数
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::InsertNode(AVLNode<K, V>* node, const K& key, const V& val)
{
	if (node == nullptr)
	{
		node = new AVLNode<K, V>(key, val);//height默认为0
		this->NodeSize++;
	}
	else
	{
		if (node->key > key)
		{
			node->left = InsertNode(node->left, key, val);
			//查看是否需要旋转
			//L插入,可能导致LR插入或者LL插入(L插入一定有node的左子树高度大于node右子树高度且需要旋转时差为2)
			if (GetNodeHeight(node->left) - GetNodeHeight(node->right) == 2)
			{
				//此时一定需要旋转,LL插入是左旋
				if (key < node->left->key)
				{
					node = SingleRotateWithRight(node);
				}
				else
				{
					//LR插入
					node = DoubleRotateWithRight(node);
				}
			}
		}
		else if (node->key < key)
		{
			node->right = InsertNode(node->right, key, val);
			//查看是否需要旋转
			//R插入,可能导致RL插入或者RR插入(R插入一定有node的右子树高度大于node左子树高度且需要旋转时差为2)
			if (GetNodeHeight(node->right) - GetNodeHeight(node->left) == 2)
			{
				//此时一定需要旋转,左旋！！！左旋！！！
				if (key > node->right->key)
				{
					node = SingleRotateWithLeft(node);
				}
				else
				{
					//RL插入
					node = DoubleRotateWithLeft(node);
				}
			}
		}
		else
		{
			//键值相同
			node->val = val;
		}
	}

	//更新节点高度
	node->height = max(GetNodeHeight(node->left), GetNodeHeight(node->right)) + 1;
	return node;
}

//删除节点的辅助函数
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::DeleteNode(AVLNode<K, V>* node, const K& key)
{
	//旋转操作会自行维护节点的高度
	if (node == nullptr) { return node; }

	if (node->key > key)
	{
		//走左边
		node->left = DeleteNode(node->left, key);
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
	else if (node->key < key)
	{
		//走右边
		node->right = DeleteNode(node->right, key);
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
			AVLNode<K, V>* tempNode = node;//保存node指针
			node = node->right;			 //直接将node换成它的子节点
			delete tempNode;			 //释放原node节点的空间
			this->NodeSize--;			 //节点数减小
		}
		else if (node->right == nullptr)
		{
			//只有左子树
			AVLNode<K, V>* tempNode = node;//保存node指针
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
				AVLNode<K, V>* preNode = FindMaxNode(node->left);
				//找到前驱节点之后,交换preNode和node的值
				node->key = preNode->key;
				node->val = preNode->val;
				//接下来继续递归,从node的左子树出发,删除tempNode->val值的节点
				//(此时要删除的节点一定属于case 1或case 2)
				node->left = DeleteNode(node->left, preNode->key);
			}
			else
			{
				//找到next的后继节点
				AVLNode<K, V>* nextNode = FindMinNode(node->right);
				//交换node和nextNode的值
				node->key = nextNode->key;
				node->val = nextNode->val;
				node->right = DeleteNode(node->right, nextNode->key);
			}
		}
	}
	return node;
}

//插入节点的函数
template<class K, class V>
void AVL<K, V>::Insert(const K& key, const V& val)
{
	this->root = InsertNode(this->root, key, val);
}

//删除节点的函数
template<class K, class V>
void AVL<K, V>::Delete(const K& key)
{
	this->root = DeleteNode(this->root, key);
}

//判断AVL中是否存在key
template<class K, class V>
bool AVL<K, V>::Search(const K& key)const
{
	AVLNode<K, V>* tempNode = this->root;
	while (tempNode != nullptr)
	{
		if (tempNode->key < key)
		{
			//走右边
			tempNode = tempNode->right;
		}
		else if (tempNode->key > key)
		{
			tempNode = tempNode->left;
		}
		else
		{
			return true;
		}
	}
	return nullptr;
}

//得到AVL中指定值的节点
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::GetNode(const K& key)const
{
	AVLNode<K, V>* tempNode = this->root;
	while (tempNode != nullptr && tempNode->key != key)
	{
		if (tempNode->key < key)
		{
			//走右边
			tempNode = tempNode->right;
		}
		else if (tempNode->key > key)
		{
			tempNode = tempNode->left;
		}
	}
	return tempNode;
}

//返回AVL中最小值的节点
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::GetMinNode()const
{
	return FindMinNode(this->root);
}

//返回AVL中最大值的节点
template<class K, class V>
AVLNode<K, V>* AVL<K, V>::GetMaxNode()const
{
	return FindMaxNode(this->root);
}

//重载[]操作符
template<class K, class V>
V& AVL<K, V>::operator[](const K& key)
{
	AVLNode<K, V>* node = GetNode(key);
	if (node == nullptr)
	{
		V val{};//C++ 11
		Insert(key, val);
		node = GetNode(key);
	}
	return node->val;
}

#endif // !AVLTREE_H
