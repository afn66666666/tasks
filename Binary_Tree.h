#pragma once
#include "exceptions_nodes_other.h"
using namespace std;



template <typename T1, typename T2>
class AC {
public:
	virtual void insert( T1 key, T2 value) = 0;
	virtual AC_Node<T1,T2>* search( T1 key) = 0;
	virtual void remove( T1 key) = 0;
	virtual ~AC() = 0;// виртуальный деструктор
};

template<class T1, class T2>
AC<T1,T2>::~AC(){
	cout << "\n\n\ndestructor of AC is called. Tree is deleted.\n\n\n";
}


template <typename T1, typename T2>
class BST : public AC<T1,T2> {
protected:
	/*struct Node {
		T1 key;
		T2 value;
		Node* left;
		Node* right;
		balance_factor bf;

	};*/

	AC_Node<T1,T2>* root;
	
	int depth_for_traverse;
	Comparator<T1> comparator_in_bts;



public:

	BST(Comparator<T1>&comparator) { // CONSTRUCT
		comparator_in_bts = comparator;
		root = nullptr;
		depth_for_traverse = 0;
		rmv = new Remove(comparator_in_bts);
		srch = new Search(comparator_in_bts);
		insrt = new Insert(comparator_in_bts);
	}

	BST(const BST& tree) {  // COPY CONSTRUCT
		this->root = copy_func(tree.root);
	}


	~BST()  {    // DESTRUCT
		delete rmv;
		delete srch;
		delete insrt;
		root = __clear_tree(root);
		/*delete root;*/
	}

protected:

	class Insert {
	public:
		friend virtual void BST<T1,T2>::Insert_template(AC_Node<T1, T2>*& root, T1 key,T2 value);
		friend BST<T1, T2>::BST(Comparator<T1>& cmp);
		Comparator<T1> cmp_in_btp;
		Insert(Comparator<T1>& comparator_from_bts) {
			cmp_in_btp = comparator_from_bts;
		}
		virtual void __insert(typename ::AC_Node<T1,T2>*& root, T1 key, T2 value) {
			if (root == nullptr) {
				root = new AC_Node<T1,T2>;
				root->key = key;
				root->value = value;
				root->left = nullptr;
				root->right = nullptr;
			}
			else {
				if (cmp_in_btp.compare(root->key, key) > 0) {
					__insert(root->right, key, value);
				}

				if (cmp_in_btp.compare(root->key, key) < 0) {
					__insert(root->left, key, value);
				}

				if (!(cmp_in_btp.compare(root->key, key))) {
					throw insert_exc("INSERT EXCEPTION : node with this key already exists.");
				}
			}

		}
		virtual void __insert_hook(AC_Node<T1,T2>*& root) {};
		//return true;
		
	};


	class Search{
		friend virtual AC_Node<T1,T2>* BST::Search_template(AC_Node<T1, T2>* root, T1 key);
		friend BST<T1, T2>::BST(Comparator<T1>& cmp);
	protected:
		Comparator<T1> cmp_for_btp;
		Search(Comparator<T1>& comparator_from_bts) {
			cmp_for_btp = comparator_from_bts;
		}
		 AC_Node<T1,T2>*& __search(AC_Node<T1,T2>* root, T1 key) { // non_virtual, search is same for BST
			if (root == nullptr) {
				throw search_exc("SEARCH EXCEPTION : node with this key not found...");
				
			}
			else {
				if (!(cmp_for_btp.compare(root->key, key))) {
					return root;
				}
				else if (cmp_for_btp.compare(root->key, key) > 0) {
					__search(root->right, key);

				}
				else if (cmp_for_btp.compare(root->key, key) < 0) {
					__search(root->left, key);

				}
			}
		 }
	};

	
	class Remove {
		friend virtual void BST::Remove_template(::AC_Node<T1,T2>*&root,T1 key);
		friend BST<T1, T2>::BST(Comparator<T1>& cmp);
	protected:
		bool found_flag = false;
		Comparator<T1> cmp_in_btp;
		Remove(Comparator<T1>& comparator_from_bts) {
			cmp_in_btp = comparator_from_bts;
		}
		virtual AC_Node<T1,T2>* __remove(AC_Node<T1,T2>*& root, T1 key){
			AC_Node<T1,T2>* tmp = root;
			if (root == nullptr) {
				if (found_flag) {
					return nullptr;
				}
				else {
					throw remove_exc("REMOVE EXCEPTION : node with this key not exist.");
				}
			}
			else if (cmp_in_btp.compare(root->key, key) > 0) {
				__remove(root->right, key);
			}
			else if (cmp_in_btp.compare(root->key, key) < 0) {
				__remove(root->left, key);
			}
			else { // we found 
				found_flag = true;
				if (root->left == nullptr and root->right == nullptr) {
					root = nullptr;
				}
				else if (root->left != nullptr and root->right != nullptr) {
					tmp = root->right;
					while (tmp->left != nullptr) { // look for  closest element for del_element (some more)
						tmp = tmp->left;
					}
					root->key = tmp->key; // change this node with root
					root->value = tmp->value;
					root->right = __remove(root->right, root->key); // delete this node (search this node again
					// with recurse help and then delete this node (2 vars...))

				}
				else {
					if (root->left != nullptr) {
						root = root->left;
					}
					else if (root->right != nullptr) {
						root = root->right;
					}
					else {
						root = nullptr;
					}
				}
			}
			return root;
			
		}


		virtual void remove_hook(AC_Node<T1,T2>*&root,T1 key) {};
	};

		
	Remove* rmv;
	Insert* insrt;
	Search* srch;

	virtual void __prefix(AC_Node<T1,T2>*root, bool (*cb_ptr)(T1 key, T2 value, int depth,int bf)){
		if (root == nullptr) {
			/*depth_for_traverse--;*/
			return;
			
		}
		else {
			cb_ptr(root->key, root->value, depth_for_traverse,root->bf.avl);
			depth_for_traverse++;
			__prefix(root->left, *cb_ptr);
			__prefix(root->right, *cb_ptr);
			depth_for_traverse--;
		}
		
	}

	virtual void __infix(AC_Node<T1,T2>* root, bool(*cb_ptr)(T1 key, T2 value, int depth,int bf)) {
		if (root == nullptr) {
			return;
		}
		else {
			depth_for_traverse++;
			__infix(root->left,*cb_ptr);
			depth_for_traverse--;
			cb_ptr(root->key, root->value, depth_for_traverse,root->bf.avl);		
			depth_for_traverse++;
			__infix(root->right, *cb_ptr);
			depth_for_traverse--;
		}
	}

	virtual void __postfix(AC_Node<T1,T2>* root, bool(*cb_ptr)(T1 key, T2 value, int depth,int bf)) {
		if (root == nullptr) {
			return;
		}
		else {
			depth_for_traverse++;
			__postfix(root->left, *cb_ptr);
			/*depth_for_traverse--;*/
			__postfix(root->right, *cb_ptr);
			depth_for_traverse--;
			cb_ptr(root->key, root->value, depth_for_traverse,root->bf.avl);
		}
	}

	virtual AC_Node<T1,T2>* copy_func(AC_Node<T1,T2>* r_root) {
		AC_Node<T1,T2>* new_tree = new AC_Node<T1,T2>;
		if (r_root == nullptr) {
			return nullptr;
		}
		else {
			
			new_tree->key = r_root->key;
			new_tree->value = r_root->value;
			new_tree->bf = r_root->bf;
			new_tree->left = copy_func(r_root->left);
			new_tree->right = copy_func(r_root->right);
			
			
		}
		return new_tree;
	}

	AC_Node<T1,T2>* __clear_tree(AC_Node<T1,T2>* root) {
		if (root != nullptr) {
			root->left = __clear_tree(root->left);

			root->right = __clear_tree(root->right);
			root = nullptr;
		}
		this->root = root;
		return root;

	}

	virtual void Remove_template(AC_Node<T1, T2>*& root, T1 key) {
		rmv->__remove(root, key);
	}
	virtual void Insert_template(AC_Node<T1, T2>*& root, T1 key, T2 value) {
		insrt->__insert(root,key,value);
	}
	virtual AC_Node<T1,T2>* Search_template(AC_Node<T1, T2>* root, T1 key) {
		/*BST<T1, T2>::srch.__search(root, key);*/
		return srch->__search(root, key);
			
	}


public:
	virtual void prefix(bool (*cb_ptr)(T1 key, T2 value, int depth,int bf)) {
		__prefix(root, cb_ptr);
	}

	virtual void infix(bool (*cb_ptr)(T1 key, T2 value, int depth,int bf)) {
		__infix(root, cb_ptr);
	}
		
	virtual void postfix(bool(*cb_ptr)(T1 key, T2 value, int depth,int bf)) {
		__postfix(root, cb_ptr);
	}

		
	virtual void remove(T1 key) override {
		Remove_template(root, key);
	}

	virtual void insert(T1 key, T2 value) override  {
		Insert_template(root, key, value);
	}

	virtual AC_Node<T1,T2>* search(T1 key) override {
		return Search_template(root, key);
	}


	void clear_tree() {
		__clear_tree(root);
	}

	BST<T1,T2> &operator = ( const BST<T1, T2>& t2) {
		this->__clear_tree(root);
		this->root = copy_func(t2.root);
		return *this;
	}
	  
	
};









