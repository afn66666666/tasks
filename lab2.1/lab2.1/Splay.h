#pragma once



using namespace std;

template <class T1, class T2>
class Splay_Tree : public BST<T1,T2> {



	class slpay_insert : public BST<T1,T2>::Insert {
		virtual void __insert(AC_Node<T1, T2>* root, T1 key, T2 value) {
		}
	};
	class splay_remove : public BST<T1, T2>::Remove {};
	class splay_search : public BST<T1, T2>::Search {};

};