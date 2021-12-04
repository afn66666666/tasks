#pragma once


template <class T1, class T2>
class Red_Black : public BST<T1, T2> {
protected:

	class rb_insert : public BST<T1,T2>::Insert {
		rb_insert(Comparator<T1>& cmp) : BST<T1, T2>::Insert(cmp) {}
		virtual void __insert(typename ::AC_Node<T1, T2>*& root, T1 key, T2 value) override {
			
		}
	};
	class rb_remove : public BST<T1, T2>::Remove {
		rb_remove(Comparator<T1>& cmp) : BST<T1, T2>::Remove(cmp) {}
	};
	rb_insert* rb_insrt;
	rb_remove* rb_rmv;

	Red_Black(Comparator<T1>& cmp) : BST<T1,T2>(cmp){
		rb_insrt = new rb_insert(cmp);
		rb_rmv = new rb_remove(cmp);
	}


};