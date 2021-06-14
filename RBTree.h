#pragma once

enum Colour
{
	RED,
	BLACK
};

template<class T>
struct RBTreeNode
{
	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	RBTreeNode<T>* _parent;
	T _t;

	enum Colour _col;

	RBTreeNode(const T& t)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _t(t)
		, _col(RED)
	{}
};

template<class T,class Ref,class Ptr>
class RBTreeIterator
{
	typedef RBTreeIterator<T, Ref, Ptr> Self;
	typedef RBTreeNode<T> Node;
	Node* _node;

	RBTreeIterator(Node* node)
		: _node(node)
	{}

	Ref operator*(const T& t)
	{
		return _node->_t;
	}

	Ptr operator->(const T& t)
	{
		return &(_node->_t);
	}
	//++it  对于后置++需要加一个int
	Self operator++()
	{

	}

	bool operator!=(const Self s)const
	{
		return _node != s._node;
	}
};



// set<K>    -> RBTree<K, K>
// map<K, V> -> RBTree<K,pair<const K, V>>
template<class K, class T, class KeyOfT>
struct RBTree
{
public:
	typedef RBTreeIterator<T, T&, T*> Iterator;
	typedef RBTreeIterator<T, const T&,const T*> Const_Iterator;
	typedef RBTreeNode<T> Node;

	//对于红黑树的Begin()迭代器来说，应该从中序遍历的第一个开始
	Iterator Begin()
	{
		Node* cur = _root;
		while (cur && cur->_left)
		{
			cur = cur->_left;
		}
		return Iterator(cur);
	}

	Iterator End()
	{
		return Iterator(nullptr);
	}


	//bool Find(const K& k);
	pair<Node*, bool> Insert(const T& t)
	{
		KeyOfT kot;

		if (_root == nullptr)
		{
			_root = new Node(t);

			_root->_col = BLACK;
			return make_pair(_root, true);
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (kot(cur->_t) < kot(t))
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (kot(cur->_t) > kot(t))
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return make_pair(cur, false);
			}
		}

		cur = new Node(t); // RED
		if (kot(parent->_t) < kot(t))
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		Node* newnode = cur;

		// 
		while (parent && parent->_col == RED)
		{
			Node* grandfather = parent->_parent;
			if (grandfather->_left == parent)
			{
				Node* uncle = grandfather->_right;
				// 情况1：u存在且为红
				if (uncle && uncle->_col == RED)
				{
					// 变色
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					// 继续往上处理
					cur = grandfather;
					parent = cur->_parent;
				}
				else //  情况2+3：u不存在或存在且为黑
				{
					//        g
					//      p
					//   c
					//
					if (cur == parent->_left)  // 
					{
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else
					{
						//        g
						//      p
						//         c
						//
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}
			else // grandfather->_right == parent
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else //  情况2+3：u不存在或存在且为黑
				{
					if (cur == parent->_right)
					{
						RotateL(grandfather);
						grandfather->_col = RED;
						parent->_col = BLACK;
					}
					else
					{
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}
		}


		_root->_col = BLACK;
		return make_pair(newnode, true);
	}


	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		Node* parentParent = parent->_parent;

		subL->_right = parent;
		parent->_parent = subL;

		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (parentParent->_left == parent)
			{
				parentParent->_left = subL;
			}
			else
			{
				parentParent->_right = subL;
			}

			subL->_parent = parentParent;
		};
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}

		subR->_left = parent;

		Node* parentParent = parent->_parent;
		parent->_parent = subR;

		if (_root == parent)
		{
			_root = subR;
		}
		else
		{
			if (parentParent->_left == parent)
			{
				parentParent->_left = subR;
			}
			else
			{
				parentParent->_right = subR;
			}
		}

		subR->_parent = parentParent;
	}

private:
	Node* _root = nullptr;
};








//#pragma once
//
//enum Color
//{
//	RED,
//	BLACK
//};
//
////对于map和set来说，就是使用第二个模板参数来控制，里面存储的类型,这相当于更高维度的泛型模板，对于红黑树里面存储的类型我交给T来控制
//template<class T>
//struct RBTreeNode
//{
//	RBTreeNode<T>* _left;
//	RBTreeNode<T>* _right;
//	RBTreeNode<T>* _parent;
//	T _t;
//	enum Color _col;
//
//
//	RBTreeNode(const T& t)
//		: _left(nullptr)
//		, _right(nullptr)
//		, _parent(nullptr)
//		, _t(t)
//		, _col(RED)
//	{}
//};
//
////这里传这个K非常的有必要
////set<K>     ->   RBTree<K,K>     对于set来说是非常清楚K是什么的
////map<K,V>   ->   RBTree<K,pair<const K,T>> 但是如果不传K的话，我知道T是pair<constK,V>类型，但是不知道K是什么
//template<class K,class T,class KeyOfT>                //这个KeyOfT就是取出T中的key值
//struct RBTree
//{
//	typedef RBTreeNode<T> Node;
//public:
//
//	//但是你这里所传的t是有问题的，对于模板来说，并不知道你所传的是Key还是pair<const Key,V>,但是这里比较是需要使用到Key来进行比较的
//	//当然你的pair也可以比较，但是需要的是pair里面的Key来进行比较
//	pair<Node*,bool> Insert(const T& t)
//	{
//		KeyOfT kot;
//		if (_root == nullptr)
//		{
//			_root = new Node(t);
//			_root->_col = BLACK;
//			return make_pair(_root, true);
//		}
//		Node* parent = nullptr;
//		Node* cur = _root;
//
//		while (cur)
//		{
//			//主要的问题就出现在这里，此时我是不知道到底这个T是K，还是pair<const K,V>中的哪一种类型，如果是K，那就好说了
//			//直接就可以取他的K值进行比较，但是如果是pair的话，是不是应该取出他的第一个参数才能进行比较呢
//
//			if (kot(cur->_t) < kot(t))
//			{
//				parent = cur;
//				cur = cur->_right;
//			}
//			else if (kot(cur->_t) > kot(t))
//			{
//				parent = cur;
//				cur = cur->_left;
//			}
//			else
//			{
//				return make_pair(cur, false);
//			}
//		}
//
//		cur = new Node(t);//RED
//		//此时说明已经找到了位置，还需要进行插入
//		if (kot(parent->_t) < kot(t))
//		{
//			//说明此时应该链接在父节点的右边
//			parent->_right = cur;
//			cur->_parent = parent;
//		}
//		else
//		{
//			parent->_left = cur;
//			cur->_parent = parent;
//		}
//
//		Node* newnode = cur;
//		//因为新节点的默认颜色是红色，因此：如果其双亲节点的颜色是黑色，没有违反红黑树任何性质，则不需要调整
//		while (parent && parent->_col == RED) //cur是新增的肯定存在parent，但是如果他是往上跳了一层的，就不会有parent了
//		{
//			Node* grandfather = parent->_parent;
//			if (grandfather->_left == parent)
//			{
//				Node* uncle = grandfather->_right;
//				//情况一：叔叔存在且为红色，第一种情况是不旋转的，只是单纯的变一个色
//				if (uncle && uncle->_col == RED)
//				{
//					//变色
//					parent->_col = uncle->_col = BLACK;
//					grandfather->_col = RED;
//
//					//继续往上处理
//					cur = grandfather;
//					parent = cur->_parent;
//				}
//				else //情况2+3 u不存在或者存在且为黑 
//				{
//					//			g
//					//       p
//					//    c
//					if (cur == parent->_left)//右单旋
//					{
//						RotateR(grandfather);
//						parent->_col = BLACK;
//						grandfather->_col = RED;
//					}
//					else
//					{
//						//此时就是一个双旋了
//						//		g
//						//    p
//						//      c
//						RotateL(parent);
//						RotateR(grandfather);
//						cur->_col = BLACK;
//						grandfather->_col = RED;
//					}
//					break;
//				}
//			}
//			else //grandfather->_right == parent
//			{
//				Node* uncle = grandfather->_left;
//				if (uncle && uncle->_col == RED)
//				{
//					uncle->_col = parent->_col = BLACK;
//					grandfather->_col = RED;
//
//					cur = grandfather;
//					parent = cur->_parent;//这里再不要忘记，也有可能迭代的要往上继续处理
//				}
//				else
//				{
//					//叔叔不存在或者叔叔的颜色为黑
//					//       g
//					//			p
//					//			   c
//					if (cur == parent->_right)
//					{
//						RotateL(grandfather);
//						grandfather->_col = RED; //
//						parent->_col = BLACK;
//					}
//					//			g
//					//			  p
//					//			c
//					else
//					{
//						RotateR(parent);
//						RotateL(grandfather);
//						cur->_col = BLACK;
//						grandfather->_col = RED;
//					}
//
//					break;
//				}
//			}
//		}
//
//		_root->_col = BLACK;//始终把根变为黑色
//		return make_pair(newnode, true);
//	}
//
//
//	void RotateR(Node* parent)
//	{
//		Node* subL = parent->_left;
//		Node* subLR = subL->_right;
//
//		parent->_left = subLR;
//		//但是subLR有可能是空的，那么下面这个代码就会崩
//		if (subLR)
//			subLR->_parent = parent;
//
//		Node* parentParent = parent->_parent;
//
//		subL->_right = parent;
//		parent->_parent = subL;
//
//
//		//此时还需要最后一步，把根结点换掉
//		if (parent == _root)
//		{
//			_root = subL;
//			_root->_parent = nullptr;
//		}
//		else
//		{
//			//此时你需要把30这个结点连接住，但是应该连接在哪一边还需要进行判断
//			if (parentParent->_left == parent)
//			{
//				parentParent->_left = subL;
//			}
//			else
//			{
//				parentParent->_right = subL;
//			}
//			subL->_parent = parentParent;
//		}
//	}
//
//	void RotateL(Node* parent)
//	{
//		Node* subR = parent->_right;
//		Node* subRL = subR->_left;
//
//		parent->_right = subRL;
//		if (subRL)
//			subRL->_parent = parent;
//
//		Node* parentParent = parent->_parent;//先保存下来，因为后面会改变这个，就找不到最开始的父节点了
//
//		subR->_left = parent;
//		parent->_parent = subR;
//
//		if (parent == _root)
//		{
//			_root = subR;
//			subR->_parent = nullptr;
//		}
//		else
//		{
//			//作为子树的一部分
//			if (parentParent->_left == parent)
//			{
//				parentParent->_left = subR;
//			}
//			else
//			{
//				parentParent->_right = subR;
//			}
//			subR->_parent = parentParent;
//		}
//	}
//
//	//void _Inorder(Node* root)
//	//{
//	//	KeyOfT kot;
//	//	if (root == nullptr)
//	//		return;
//	//	_Inorder(root->_left);
//	//	cout << kot(root->_t)<< " ";
//	//	_Inorder(root->_right);
//	//}
//
//	//void Inorder()
//	//{
//	//	_Inorder(_root);
//	//	cout << endl;
//	//}
//
//	//bool _CheckRedCol(Node* root)
//	//{
//	//	if (root == nullptr)
//	//		return true;
//	//	if (root->_col == RED)
//	//	{
//	//		Node* parent = root->_parent;
//	//		if (parent->_col == RED)
//	//		{
//	//			cout << "违反规则3：存在连续的红结点" << endl;
//	//			return false;
//	//		}
//	//	}
//	//	return _CheckRedCol(root->_left) && _CheckRedCol(root->_right);//其实拆分为了根，左子树，右子树的问题
//	//}
//
//	//bool _CheckBlackNum(Node* root,int blackNum,int trueNum)
//	//{
//	//	if (root == nullptr)
//	//	{
//	//		return trueNum == blackNum;//算出来的和真实的进行比较
//	//	}
//
//	//	if (root->_col == BLACK)
//	//	{
//	//		blackNum++;
//	//	}
//
//	//	return _CheckBlackNum(root->_left, blackNum,trueNum) && _CheckBlackNum(root->_right, blackNum,trueNum);
//	//}
//	////不要考虑去检查最长路径是否不超过最短路径的两倍，这个方法麻烦
//	////而是从反向进行思考，检查他的性质是否都满足
//	//bool IsBalance()
//	//{
//	//	if (_root && _root->_col == RED)
//	//	{
//	//		cout << "违反规则1：根结点是红色的" << endl;
//	//		return false;
//	//	}
//
//	//	int trueNum = 0; //拿到真实路径上面的结点值，然后和算出来的进行比较
//	//	Node* cur = _root;
//	//	while (cur)
//	//	{
//	//		if (cur->_col == BLACK)
//	//		{
//	//			++trueNum;
//	//		}
//	//		cur = cur->_left;
//	//	}
//	//	int blackNum = 0;
//	//	return _CheckRedCol(_root) && _CheckBlackNum(_root, blackNum,trueNum); //这样就知道是否满足了红黑树的所有性质
//	//}
//
//private:
//	Node* _root = nullptr;
//};