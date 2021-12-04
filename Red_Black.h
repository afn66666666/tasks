#pragma once


template <class T1, class T2>
class Red_Black : public BST<T1, T2> {
public:
	//CONSTRUCTOR
	Red_Black(Comparator<T1>& cmp) : BST<T1, T2>(cmp) {
		rb_insrt = new rb_insert(cmp);
		rb_rmv = new rb_remove(cmp);
	}

	~Red_Black() {
		delete rb_insrt;
		delete rb_rmv;
	}


	virtual void prefix(bool (*cb_ptr)(T1 key, T2 value, int depth, bool rb)) {
		if (BST<T1, T2>::root == nullptr) {
			cout << "\nPREFIX : tree is not created.\n" << endl;
		}
		if (BST<T1,T2>::root->right->value == NULL) {
			cout << "\ntree is empty\n";
			return;
		}
		__prefix(BST<T1,T2>::root->right, cb_ptr);
	
	}
	virtual AC_Node<T1,T2>* search(T1 key)override {
		if (BST<T1,T2>::root == nullptr) {
			throw insert_exc("RB EXCEPTION : tree is not exist...");
		}
		return BST<T1, T2>::Search_template(BST<T1, T2>::root->right, key);
	}


protected:
	virtual void Insert_template(AC_Node<T1, T2>*& root, T1 key, T2 value) override {
		rb_insrt->__insert(root, key, value);
		rb_insrt->__insert_hook(root);
	}
	virtual void Remove_template(AC_Node<T1, T2>*& root, T1 key) override {
		rb_rmv->__remove(root, key);
	}

	virtual void __prefix(AC_Node<T1, T2>* root, bool (*cb_ptr)(T1 key, T2 value, int depth, bool rb)){
		if (root == nullptr) {
			/*depth_for_traverse--;*/
			return;

		}
		else {
			cb_ptr(root->key, root->value, BST<T1,T2>::depth_for_traverse, root->bf.rb);
			BST<T1,T2>::depth_for_traverse++;
			__prefix(root->left, *cb_ptr);
			__prefix(root->right, *cb_ptr);
			BST<T1, T2>::depth_for_traverse--;
		}
	}


	//INSERT CLASS
	class rb_insert : public BST<T1,T2>::Insert {
	protected:
		friend Red_Black::Red_Black(Comparator<T1>& cmp);
		friend void Red_Black::Insert_template(AC_Node<T1, T2>*& root, T1 key, T2 value);

		rb_insert(Comparator<T1>& cmp) : BST<T1, T2>::Insert(cmp) {
			cmp_in_rb = cmp;
		}

		Comparator<T1>cmp_in_rb;


		// for inserting
		AC_Node<T1, T2>* NIL = new AC_Node<T1, T2>{ NULL,NULL,nullptr,nullptr,false };
		AC_Node<T1, T2>* node_for_insert;
		AC_Node<T1, T2>* parent_of_nfi;
		AC_Node<T1, T2>* grandparent_of_nfi;
		AC_Node<T1, T2>* grand_grand;
		AC_Node<T1, T2>* result_of_switch;
		
		//for searching
		AC_Node<T1, T2>* searcher; // p
		AC_Node<T1, T2>* parent_of_searcher; // q


		// for turns
		AC_Node<T1, T2>* temp_switch;
		AC_Node<T1, T2>* parent_of_temp_switch;
		AC_Node<T1, T2>* parent_of_node_in_switch;
		AC_Node<T1, T2>* parent_of_temp_switch_son;

		list<AC_Node<T1, T2>*> nodes_stack; // stack for rebalancing
		int connector_flag = 0;

		AC_Node<T1, T2>* parent(AC_Node<T1, T2>* node,AC_Node<T1,T2>*root) {
			
			if (root->right == nullptr) { // if tree is empty
				return nullptr;
			}
			if (node == nullptr) {
				return nullptr;
			}
			
			searcher = root->right;

			if (searcher == node) { // if node is root
				return nullptr;
			}

			while (cmp_in_rb.compare(searcher->key, node->key) != 0) {
				if (searcher == nullptr) {
					throw remove_exc("RB PARENT EXCEPTOIN : cant find a grandpa...");
				}
				if (cmp_in_rb.compare(searcher->key, node->key) > 0) {
					if (searcher->left == node or searcher->right == node) {
						return searcher;
					}
					else {
						searcher = searcher->right;
					}
				}
				else if (cmp_in_rb.compare(searcher->key, node->key) < 0) {
					if (searcher->left == node or searcher->right == node) {
						return searcher;
					}
					else {
						searcher = searcher->left;
					}
				}
			}
			return nullptr;

		}
		AC_Node<T1,T2>*leftRotate(AC_Node<T1, T2>* node, AC_Node<T1, T2>* root) {
			temp_switch = node->right;
			parent_of_temp_switch = parent(temp_switch,root);
			parent_of_node_in_switch = parent(node,root);


			parent_of_temp_switch = parent_of_node_in_switch;
			if (parent_of_node_in_switch != nullptr) {
				if (parent_of_node_in_switch->left == node) {
					parent_of_node_in_switch->left = temp_switch;
				}
				else {
					parent_of_node_in_switch->right = temp_switch;
				}
			}
			node->right = temp_switch->left;
			if (temp_switch->left != nullptr) {
				parent_of_temp_switch_son = parent(temp_switch->left,root);
				parent_of_temp_switch_son = node;
			}
			parent_of_node_in_switch = temp_switch;
			temp_switch->left = node;
			return temp_switch;
		}
		AC_Node<T1, T2>* rightRotate(AC_Node<T1, T2>* node,AC_Node<T1,T2>*root) {
			temp_switch = node->left;
			parent_of_node_in_switch = parent(node,root);
			parent_of_temp_switch = parent(temp_switch,root);


			parent_of_temp_switch = parent_of_node_in_switch;
			if (parent_of_node_in_switch != nullptr) {
				if (parent_of_node_in_switch->left == node) {
					parent_of_node_in_switch->left = temp_switch;
				}
				else {
					parent_of_node_in_switch->right = temp_switch;
				}
			}

			node->left = temp_switch->right;
			if (temp_switch->right != nullptr) {
				parent_of_temp_switch_son = parent(temp_switch->right,root);
				parent_of_temp_switch_son = node;
			}
			parent_of_node_in_switch = temp_switch;
			temp_switch->right = node;
			return temp_switch;
		}
		

		virtual void __insert(typename ::AC_Node<T1, T2>*& root, T1 key, T2 value) override {
			if (root == nullptr) {
				root = new AC_Node<T1, T2>;
				root->left = nullptr;
				root->right = nullptr;
			}

			node_for_insert = new AC_Node<T1, T2>;
			node_for_insert->key = key;
			node_for_insert->value = value;
			node_for_insert->left = NIL;
			node_for_insert->right = NIL;
			node_for_insert->bf.rb = true;

			if(root->right == nullptr){ // if tree is empty;
				root->right = node_for_insert;
				parent_of_nfi = NIL;
			}

			else {
				searcher = root->right;
				parent_of_searcher = NIL;
				while (searcher != NIL) {
					parent_of_searcher = searcher;
					if(cmp_in_rb.compare(searcher->key, node_for_insert->key ) > 0){
						searcher = searcher->right;
					}
					else if (cmp_in_rb.compare(searcher->key, node_for_insert->key) == 0) {
						throw insert_exc("RB INSERT EXCEPTION : this element already exist");
					}
					else {
						searcher = searcher->left;
					}
				}
				parent_of_nfi = parent_of_searcher;
				if (cmp_in_rb.compare(parent_of_searcher->key,node_for_insert->key) > 0) {
					parent_of_searcher->right = node_for_insert;
				}
				else {
					parent_of_searcher->left = node_for_insert;
				}

				
			}
		}
		virtual void __insert_hook(typename ::AC_Node<T1, T2>*& root) override {
			if (root->right == node_for_insert) {
				node_for_insert->bf.rb = false;
				return;
			}
			else {
				grandparent_of_nfi = parent(parent_of_nfi,root);
				while (parent_of_nfi != nullptr and parent_of_nfi->bf.rb == true) { // while father is red
					if (grandparent_of_nfi->left == parent_of_nfi and grandparent_of_nfi != nullptr) { // if father is left child
						if (grandparent_of_nfi->right != nullptr and grandparent_of_nfi->right->bf.rb == true) { // if nfi has red UNCLE
								parent_of_nfi->bf.rb = false;// parent is black
								grandparent_of_nfi->right->bf.rb = false; // uncle = black;
								grandparent_of_nfi->bf.rb = true; //grandparent is red
								node_for_insert = grandparent_of_nfi; // t = grandpa

								// отсебятина (обновление отца и деда)
								parent_of_nfi = parent(node_for_insert, root);
								grandparent_of_nfi = parent(parent_of_nfi, root);
							
						}
						else {
							if (parent_of_nfi->right == node_for_insert) { // if t is right son
								node_for_insert = parent_of_nfi; // t is parent

								//отсебятина  (обновление отца и деда)
								parent_of_nfi = parent(node_for_insert, root); //
								grandparent_of_nfi = parent(parent_of_nfi, root);


								result_of_switch = leftRotate(node_for_insert, root);

								parent_of_nfi = parent(node_for_insert, root); //
								grandparent_of_nfi = parent(parent_of_nfi, root);
							}

							parent_of_nfi->bf.rb = false; //parent black
							grandparent_of_nfi->bf.rb = true; // grandparent red

							// отсебятина
							grand_grand = parent(grandparent_of_nfi, root);
							connector_flag = 0; // отсебятина
							if (grand_grand != nullptr) { // если есть дедушка не корень
								connector_flag = 1;
							}

							result_of_switch = rightRotate(grandparent_of_nfi, root);



							if (connector_flag == 0) { // flag not change -> grandpa is root
								root->right = result_of_switch;
							}

							parent_of_nfi = parent(node_for_insert, root); //
							grandparent_of_nfi = parent(parent_of_nfi, root);

						}
					}

					else { // if father is right child;
					if (grandparent_of_nfi->left != nullptr and grandparent_of_nfi->left->bf.rb == true) { // if we have UNCLE RED
						parent_of_nfi->bf.rb = false; // father is black
						grandparent_of_nfi->left->bf.rb = false; // uncle is black
						grandparent_of_nfi->bf.rb = true; // grandparent is red
						node_for_insert = grandparent_of_nfi;

						//отсебятина (обновление отца и деда)
						parent_of_nfi = parent(node_for_insert, root);
						grandparent_of_nfi = parent(parent_of_nfi, root);
					}
					else { // UNCLE not exist or uncle black???
						if (parent_of_nfi->left == node_for_insert) { // if t is left son
							node_for_insert = parent_of_nfi; // t is parent

							//отсебятина (обновление отца и деда)
							parent_of_nfi = parent(node_for_insert, root);
							grandparent_of_nfi = parent(parent_of_nfi, root);



							result_of_switch = rightRotate(node_for_insert, root);

							//отсебятина
							parent_of_nfi = parent(node_for_insert, root);
							grandparent_of_nfi = parent(parent_of_nfi, root);
						}

						parent_of_nfi->bf.rb = false; // parent is black
						grandparent_of_nfi->bf.rb = true; // grandparent is red


						// отсебятина
						connector_flag = 0;
						grand_grand = parent(grandparent_of_nfi, root);
						if (grand_grand != nullptr) {
							connector_flag = 1;
						}

						// norm
						result_of_switch = leftRotate(grandparent_of_nfi, root);
						//отсебятина
						if (connector_flag == 0) {
							root->right = result_of_switch;
						}

						//отсебятина
						parent_of_nfi = parent(node_for_insert, root);
						grandparent_of_nfi = parent(parent_of_nfi, root);
					}

					}
				}
				root->right->bf.rb = false;
			}

		}

	};


	//REMOVE CLASS
	class rb_remove : public BST<T1, T2>::Remove {
		friend Red_Black::Red_Black(Comparator<T1>& cmp);
		friend void Red_Black::Remove_template(AC_Node<T1, T2>*& root, T1 key);
		Comparator<T1>cmp_in_rb;
		rb_remove(Comparator<T1>& cmp) : BST<T1, T2>::Remove(cmp) {}

	protected:
		AC_Node<T1, T2>* parent(AC_Node<T1, T2>* node, AC_Node<T1, T2>* root) {

			if (root->right->value == NULL) { // if tree is empty
				return NIL;
			}
			if (node->value == NULL) {
				return NIL;
			}

			searcher = root->right;

			if (searcher == node) { // if node is root
				return NIL;
			}

			while (cmp_in_rb.compare(searcher->key, node->key) != 0) {
				if (searcher->value == NULL) {
					throw remove_exc("RB PARENT EXCEPTOIN : cant find a grandpa...");
				}
				if (cmp_in_rb.compare(searcher->key, node->key) > 0) {
					if (searcher->left == node or searcher->right == node) {
						return searcher;
					}
					else {
						searcher = searcher->right;
					}
				}
				else if (cmp_in_rb.compare(searcher->key, node->key) < 0) {
					if (searcher->left == node or searcher->right == node) {
						return searcher;
					}
					else {
						searcher = searcher->left;
					}
				}
			}
			return NIL;

		}
		AC_Node<T1, T2>* minimum(AC_Node<T1, T2>* node) {
			while (node->left->value != NULL) {
				node = node->left;
			}
			return node;
		}
		AC_Node<T1, T2>* successor(AC_Node<T1, T2>* node,AC_Node<T1,T2>*root) {
			if (node->right->value != NULL) {
				return minimum(node->right);
			}
			searcher = parent(node, root);
			while (searcher->value != NULL and node == searcher->right) {
				node = y;
				y = parent(y, root);
			}
			return searcher;
		}
		AC_Node<T1, T2>* leftRotate(AC_Node<T1, T2>* node, AC_Node<T1, T2>* root) {
			temp_switch = node->right;
			parent_of_temp_switch = parent(temp_switch, root);
			parent_of_node_in_switch = parent(node, root);


			parent_of_temp_switch = parent_of_node_in_switch;
			if (parent_of_node_in_switch->value != NULL) {
				if (parent_of_node_in_switch->left == node) {
					parent_of_node_in_switch->left = temp_switch;
				}
				else {
					parent_of_node_in_switch->right = temp_switch;
				}
			}
			node->right = temp_switch->left;
			if (temp_switch->left->value != NULL) {
				parent_of_temp_switch_son = parent(temp_switch->left, root);
				parent_of_temp_switch_son = node;
			}
			parent_of_node_in_switch = temp_switch;
			temp_switch->left = node;
			return temp_switch;
		}
		AC_Node<T1, T2>* rightRotate(AC_Node<T1, T2>* node, AC_Node<T1, T2>* root) {
			temp_switch = node->left;
			parent_of_node_in_switch = parent(node, root);
			parent_of_temp_switch = parent(temp_switch, root);


			parent_of_temp_switch = parent_of_node_in_switch;
			if (parent_of_node_in_switch->value != NULL) {
				if (parent_of_node_in_switch->left == node) {
					parent_of_node_in_switch->left = temp_switch;
				}
				else {
					parent_of_node_in_switch->right = temp_switch;
				}
			}

			node->left = temp_switch->right;
			if (temp_switch->right->value != NULL) {
				parent_of_temp_switch_son = parent(temp_switch->right, root);
				parent_of_temp_switch_son = node;
			}
			parent_of_node_in_switch = temp_switch;
			temp_switch->right = node;
			return temp_switch;
		}


		AC_Node<T1, T2>* NIL = new AC_Node<T1,T2>{NULL,nullptr,NULL,nullptr,false};

		
		
		
		AC_Node<T1, T2>* parent_of_nfr = NIL; 
		AC_Node<T1, T2>* y_parent = NIL;
		AC_Node<T1, T2>* x_parent = NIL;
		AC_Node<T1, T2>* z = NIL;
		AC_Node<T1, T2>* result_of_switch = NIL;
		AC_Node<T1, T2>* x = NIL;
		AC_Node<T1, T2>* y = NIL;
		AC_Node<T1, T2>* brother = NIL;
		

		AC_Node<T1, T2>* searcher = NIL;

		//for turns
		AC_Node<T1, T2>* temp_switch = NIL;
		AC_Node<T1, T2>* parent_of_temp_switch = NIL;
		AC_Node<T1, T2>* parent_of_node_in_switch = NIL;
		AC_Node<T1, T2>* parent_of_temp_switch_son = NIL;
		
		int connector_flag = 0;


		virtual AC_Node<T1, T2>* __remove(AC_Node<T1, T2>*& root, T1 key) override {
			if (root == nullptr or root->right == nullptr) {
				throw remove_exc("RB REMOVE EXCEPTION : tree is empty");
			}

			z = root->right;
			parent_of_nfr = NIL;
			while (cmp_in_rb.compare(z->key, key) != 0) {
				if (z == nullptr) {
					throw remove_exc("RB REMOVE EXCEPTION : trying to delete non-existent element");
				}
				parent_of_nfr = z;
				if (cmp_in_rb.compare(z->key, key) > 0){
					z = z->right;
				}
				else if (cmp_in_rb.compare(z->key, key) < 0) {
					z = z->left;
				}
			}

			// if node without children
			if (z->left->value == NULL or z->right->value == NULL) {
				y = z;    // y = z

			} 
			
			//2 NODES
			else {
				y = successor(z, root);
			}

			// selfmade
 			y_parent = parent(y, root);

			if(y->left->value != NULL){
				x = y->left;
			}
			else {
				x = y->right;
			}
			x_parent = y_parent;

			if (y_parent->value == NULL) {
				root->right = x;
			}
			else if (y == y_parent->left) {
				y_parent->left = x;
			}
			else {
				y_parent->right = x;
			}


			if (y != z) {
				z->key = y->key;
				z->value = y->value;
			}
			if (y->bf.rb == false) {
				__remove_hook(root);
			}
			 
		}
		virtual void __remove_hook(AC_Node<T1,T2>*root) {
			while (x != root->right and x->bf.rb == false) {
				if (x == x_parent->left) {
					brother = x_parent->right;
					if (brother->bf.rb == true) {
						brother->bf.rb = false;
						x_parent->bf.rb = true;
						
						//selfmade
						connector_flag = 0;
						if (root->right == x_parent) {
							connector_flag = 1;
						}
						//selfmade

						result_of_switch = leftRotate(x_parent, root);
						if (connector_flag) {
							root->right = result_of_switch;
						}
						//x_parent = parent(x, root);
						brother = x_parent->right;
					}
					if (brother->left->bf.rb == false and brother->right->bf.rb == false) {
						brother->bf.rb = true;
						x = x_parent;
						x_parent = parent(x, root);
					}
					else {
						if (brother->right->bf.rb == false) {
							brother->left->bf.rb = false;
							brother->bf.rb = true;

							//selfmade
							connector_flag = 0;
							if (root->right == brother) {
								connector_flag = 1;
							}
							//selfmade

							result_of_switch = rightRotate(brother, root);
							if (connector_flag) {
								root->right = result_of_switch;
							}

							brother = x_parent->right;
						}

						brother->bf.rb = x_parent->bf.rb;
						x_parent->bf.rb = false;
						brother->right->bf.rb = false;

						//selfmade
						connector_flag = 0;
						if (root->right == x_parent) {
							connector_flag = 1;
						}
						//selfmade
						result_of_switch = leftRotate(x_parent, root);
						if (connector_flag) {
							root->right = result_of_switch;
						}
						x = root->right;
						//TODO : root after switch...
					}
					
				}
				else {
					brother = x_parent->left;
					if (brother->bf.rb == true) {
						brother->bf.rb = false;
						x_parent->bf.rb = true;

						//selfmade
						connector_flag = 0;
						if (root->right == x_parent) {
							connector_flag = 1;
						}
						//selfmade

						result_of_switch = rightRotate(x_parent, root);
						if (connector_flag) {
							root->right = result_of_switch;
						}
						//x_parent = parent(x, root);
						brother = x_parent->left;

					}

					if (brother->left->bf.rb == false and brother->right->bf.rb == false) {
						brother->bf.rb = true;
						x = x_parent;
						x_parent = parent(x, root);
					}
					else {
						if (brother->left->bf.rb == false) {
							brother->right->bf.rb = false;
							brother->bf.rb = true;

							//selfmade
							connector_flag = 0;
							if (root->right == x_parent) {
								connector_flag = 1;
							}
							//selfmade
							result_of_switch = leftRotate(brother, root);
							if (connector_flag) {
								root->right = result_of_switch;
							}
							brother = x_parent->left;
						}
						brother->bf.rb = x_parent->bf.rb;
						x_parent->bf.rb = false;
						brother->left->bf.rb = false;
						//selfmade
						connector_flag = 0;
						if (root->right == x_parent) {
							connector_flag = 1;
						}
						//selfmade
						result_of_switch = rightRotate(x_parent, root);
						if (connector_flag) {
							root->right = result_of_switch;
						}
						x = root->right;
					}
					
				}
			}
			x->bf.rb = false;
		}
	};
	rb_insert* rb_insrt;
	rb_remove* rb_rmv;




};