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
	//++it  ���ں���++��Ҫ��һ��int
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

	//���ں������Begin()��������˵��Ӧ�ô���������ĵ�һ����ʼ
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
				// ���1��u������Ϊ��
				if (uncle && uncle->_col == RED)
				{
					// ��ɫ
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					// �������ϴ���
					cur = grandfather;
					parent = cur->_parent;
				}
				else //  ���2+3��u�����ڻ������Ϊ��
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
				else //  ���2+3��u�����ڻ������Ϊ��
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
////����map��set��˵������ʹ�õڶ���ģ����������ƣ�����洢������,���൱�ڸ���ά�ȵķ���ģ�壬���ں��������洢�������ҽ���T������
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
////���ﴫ���K�ǳ����б�Ҫ
////set<K>     ->   RBTree<K,K>     ����set��˵�Ƿǳ����K��ʲô��
////map<K,V>   ->   RBTree<K,pair<const K,T>> �����������K�Ļ�����֪��T��pair<constK,V>���ͣ����ǲ�֪��K��ʲô
//template<class K,class T,class KeyOfT>                //���KeyOfT����ȡ��T�е�keyֵ
//struct RBTree
//{
//	typedef RBTreeNode<T> Node;
//public:
//
//	//����������������t��������ģ�����ģ����˵������֪������������Key����pair<const Key,V>,��������Ƚ�����Ҫʹ�õ�Key�����бȽϵ�
//	//��Ȼ���pairҲ���ԱȽϣ�������Ҫ����pair�����Key�����бȽ�
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
//			//��Ҫ������ͳ����������ʱ���ǲ�֪���������T��K������pair<const K,V>�е���һ�����ͣ������K���Ǿͺ�˵��
//			//ֱ�ӾͿ���ȡ����Kֵ���бȽϣ����������pair�Ļ����ǲ���Ӧ��ȡ�����ĵ�һ���������ܽ��бȽ���
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
//		//��ʱ˵���Ѿ��ҵ���λ�ã�����Ҫ���в���
//		if (kot(parent->_t) < kot(t))
//		{
//			//˵����ʱӦ�������ڸ��ڵ���ұ�
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
//		//��Ϊ�½ڵ��Ĭ����ɫ�Ǻ�ɫ����ˣ������˫�׽ڵ����ɫ�Ǻ�ɫ��û��Υ��������κ����ʣ�����Ҫ����
//		while (parent && parent->_col == RED) //cur�������Ŀ϶�����parent���������������������һ��ģ��Ͳ�����parent��
//		{
//			Node* grandfather = parent->_parent;
//			if (grandfather->_left == parent)
//			{
//				Node* uncle = grandfather->_right;
//				//���һ�����������Ϊ��ɫ����һ������ǲ���ת�ģ�ֻ�ǵ����ı�һ��ɫ
//				if (uncle && uncle->_col == RED)
//				{
//					//��ɫ
//					parent->_col = uncle->_col = BLACK;
//					grandfather->_col = RED;
//
//					//�������ϴ���
//					cur = grandfather;
//					parent = cur->_parent;
//				}
//				else //���2+3 u�����ڻ��ߴ�����Ϊ�� 
//				{
//					//			g
//					//       p
//					//    c
//					if (cur == parent->_left)//�ҵ���
//					{
//						RotateR(grandfather);
//						parent->_col = BLACK;
//						grandfather->_col = RED;
//					}
//					else
//					{
//						//��ʱ����һ��˫����
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
//					parent = cur->_parent;//�����ٲ�Ҫ���ǣ�Ҳ�п��ܵ�����Ҫ���ϼ�������
//				}
//				else
//				{
//					//���岻���ڻ����������ɫΪ��
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
//		_root->_col = BLACK;//ʼ�հѸ���Ϊ��ɫ
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
//		//����subLR�п����ǿյģ���ô�����������ͻ��
//		if (subLR)
//			subLR->_parent = parent;
//
//		Node* parentParent = parent->_parent;
//
//		subL->_right = parent;
//		parent->_parent = subL;
//
//
//		//��ʱ����Ҫ���һ�����Ѹ���㻻��
//		if (parent == _root)
//		{
//			_root = subL;
//			_root->_parent = nullptr;
//		}
//		else
//		{
//			//��ʱ����Ҫ��30����������ס������Ӧ����������һ�߻���Ҫ�����ж�
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
//		Node* parentParent = parent->_parent;//�ȱ�����������Ϊ�����ı���������Ҳ����ʼ�ĸ��ڵ���
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
//			//��Ϊ������һ����
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
//	//			cout << "Υ������3�����������ĺ���" << endl;
//	//			return false;
//	//		}
//	//	}
//	//	return _CheckRedCol(root->_left) && _CheckRedCol(root->_right);//��ʵ���Ϊ�˸�����������������������
//	//}
//
//	//bool _CheckBlackNum(Node* root,int blackNum,int trueNum)
//	//{
//	//	if (root == nullptr)
//	//	{
//	//		return trueNum == blackNum;//������ĺ���ʵ�Ľ��бȽ�
//	//	}
//
//	//	if (root->_col == BLACK)
//	//	{
//	//		blackNum++;
//	//	}
//
//	//	return _CheckBlackNum(root->_left, blackNum,trueNum) && _CheckBlackNum(root->_right, blackNum,trueNum);
//	//}
//	////��Ҫ����ȥ����·���Ƿ񲻳������·������������������鷳
//	////���Ǵӷ������˼����������������Ƿ�����
//	//bool IsBalance()
//	//{
//	//	if (_root && _root->_col == RED)
//	//	{
//	//		cout << "Υ������1��������Ǻ�ɫ��" << endl;
//	//		return false;
//	//	}
//
//	//	int trueNum = 0; //�õ���ʵ·������Ľ��ֵ��Ȼ���������Ľ��бȽ�
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
//	//	return _CheckRedCol(_root) && _CheckBlackNum(_root, blackNum,trueNum); //������֪���Ƿ������˺��������������
//	//}
//
//private:
//	Node* _root = nullptr;
//};