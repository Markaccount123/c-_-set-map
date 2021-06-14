#pragma once
#include "RBTree.h"

namespace wzy
{
	template<class K>
	class set
	{
		struct SetKOfT
		{
			const K& operator()(const K& k)
			{
				return k;
			}
		};
	public:
		//对于Myset来说，自己是没有东西的，使用的是RBTree的iterator
		typedef RBTree<K, K, SetKOfT>::Iterator iterator;
		pair<RBTreeNode<K>*, bool> insert(const K& k)
		{
			return _t.Insert(k);
		}

		iterator begin()
		{
			return _t.Begin();
		}


		iterator end()
		{
			return _t.End();
		}

		
	private:
		RBTree<K, K, SetKOfT> _t;
	};

	void test_set()
	{
		set<int> s;
		s.insert(1);
		s.insert(2);
		s.insert(3);

		set<int>::iterator it = s.begin();
		while (it != s.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;

	}
}
