#include <iostream>
#include <algorithm>
#include "BSTree.h"
#include "AVLTree.h"
#include "../../BigInteger(大数类)/BigInteger(大数类)/BigInteger.h"
using namespace std;

int main()
{
	AVL<BigInteger> t;
	for (int i = 0; i < 10; i++)
	{
		BigInteger num;
		cin >> num;
		t.Insert(num);
		cout << t.getHeight() << endl;
	}
	cout << "中序遍历是:  ";
	t.inOrder([](TreeNode<BigInteger>* node) {
		cout << node->val << endl;
		});

	vector<int> nums = { 16,6,64,4,7,23,87,5,44,71,92,99 };
	AVL<int> tree;
	for (int num : nums)
	{
		tree.Insert(num);
	}
	cout << tree.getHeight() << endl;
	tree.Delete(64);
	cout << tree.getHeight() << endl;
	cout << "前序遍历是:  ";
	tree.preOrder([](TreeNode<int>* node) {
		cout << node->val << " ";
		});
	cout << endl;
	cout << "中序遍历是:  ";
	tree.inOrder([](TreeNode<int>* node) {
		cout << node->val << " ";
		});
	//int arr[] = { 21,6 ,3 ,52, 36, 59 ,66 ,61 ,63 ,88 ,80 };
	//BST<int> bst;
	//srand(time(NULL));
	//for (int i = 0; i < 11; i++)
	//{
	//	bst.Insert(arr[i]);
	//}
	//cout << "前序遍历是:  ";
	//bst.PreOrder([](TreeNode<int>* node) {
	//	cout << node->val << " ";
	//	});
	//cout << endl;
	//cout << "中序遍历是:  ";
	//bst.InOrder([](TreeNode<int>* node) {
	//	cout << node->val << " ";
	//	});
	//cout << endl;
	//cout << "后序遍历是:  ";
	//bst.BackOrder([](TreeNode<int>* node) {
	//	cout << node->val << " ";
	//	});
	//cout << endl;
	//cout << bst.Get_Tree_Height() << endl;

	//bst.Delete(52);
	//bst.Delete(66);
	//bst.Delete(63);
	//cout << "中序遍历是:  ";
	//bst.InOrder([](TreeNode<int>* node) {
	//	cout << node->val << " ";
	//	});
	//cout << endl;
	return 0;
}
