#pragma once
#include<list>
using namespace std;


template<class T1,class T2>
class AVL : public BST<T1, T2> {
public:
	//Comparator<T1>cmp_in_avl;
	AVL(Comparator<T1>& cmp) : BST<T1, T2>(cmp) {
		avl_rmv = new avl_remove(cmp);
		avl_insrt = new avl_insert(cmp);
	}
	
protected:
	
	class avl_remove : public BST<T1, T2>::Remove {
		Comparator<T1>remove_avl;
		avl_remove(Comparator<T1>& cmp) : Remove(cmp) {}
	protected:
		typename ::AC_Node<T1,T2>* Q; // for switch
		typename ::AC_Node<T1,T2>* T; // head
		typename ::AC_Node<T1,T2>* S; // trash
		typename ::AC_Node<T1,T2>* P; // node for delete or for change
		typename ::AC_Node<T1,T2>* P_par; // parent P
		typename ::AC_Node<T1,T2>* Z; // trash
		typename ::AC_Node<T1,T2>* temp; // node for delete
		typename ::AC_Node<T1,T2>* temp_par; // parent for temp
		list<AC_Node<T1,T2>*> nodes_stack; // stack for rebalancing
		int a; // a from Knut's algorithm

		friend AVL::AVL(Comparator<T1>& cmp);
		friend void AVL::Remove_template(AC_Node<T1, T2>*& root, T1 key);
		virtual ::AC_Node<T1,T2>* __remove(::AC_Node<T1,T2>*& root, T1 key) override {
			P_par = root->right;
			if (root == nullptr) {
				throw remove_exc("EXCEPTION : tree not created ...");
			}
			if (root->right == nullptr) {
				throw remove_exc("EXCEPTION : tree is empty");
			}
			else {
				P = root->right;
				T = root->right;
				while (remove_avl.compare(P->key, key) != 0) {
					if (P == nullptr) {
						throw remove_exc("EXCEPTION : element not found");
					}
					nodes_stack.push_back(P);
					if (remove_avl.compare(P->key, key) > 0) { //go right
						P_par = P;
						Q = P->right; // А4
						if (Q == nullptr) {  // this element doesnt exist
							throw exception(" this element doesnt exist");
						}
						else {
							if (Q->bf.avl != 0) {
								T = P;
								S = Q;
							}
						}
						P = Q;
						continue;
					}
					if (remove_avl.compare(P->key, key) < 0) { //go left
 						P_par = P;
						Q = P->left; // А3
						if (Q == nullptr) {
							throw remove_exc(" this element doesnt exist");
						}
						else {
							if (Q->bf.avl != 0) {
								T = P;
								S = Q;
							}

						}
						P = Q;
						continue;
					}
				} 
				
					// P - Node FOR change
					// temp - Node for delete
				// нашли узел для замены P, P_par - parent of P;
				    // просто удаляем узел и скидываем P_par на null
					if (P->left == nullptr and P->right == nullptr) {
						if (P_par == P) {
							P_par = nullptr;
							root->right = nullptr;
						}
						else {
							if (P_par->left == P) {
								P_par->left = nullptr;
								temp = nullptr;
							}
							else {

								P_par->right = nullptr;
								temp = nullptr;
							}
						}
					}

					// есть оба поддерева
					else if (P->left != nullptr and P->right != nullptr) {
						// кладем вершины от P до удаляемого узла в stack
						nodes_stack.push_back(P);
						temp = P->right;
						temp_par = P;
						while (temp->left != nullptr) {
							nodes_stack.push_back(temp);
							if (temp->left->left == nullptr) {
								temp_par = temp;
								//nodes_stack.push_back(temp_par);
							}
							temp = temp->left; // have only right tree
				
						}
						P->key = temp->key;
						P->value = temp->value;
						/*temp_par->left = temp->right;*/
						if (temp->right == nullptr) {
							if (temp_par->right == temp) {
								temp_par->right = nullptr;
							}
							else if (temp_par->left == temp) {
								temp_par->left = nullptr;
							}
							temp = nullptr;
							
						}
						// if temp has right child
						else if(temp->right!=nullptr){
							if (temp_par->right == temp) {
								temp_par->right = temp->right;
							}
							else if (temp_par->left == temp) {
								temp_par->left = temp->right;
							}
							temp = temp->right;
							
						}
					}

					// nodes with one child;
					else if (P->left != nullptr) {
						if (root->right == P) {
							P = P->left;
							root->right = P;
						}
						else {
							/*nodes_stack.push_back(P);*/
							//P_par = P;
							
							if (P_par->left == P) {
								P = P->left;
								P_par->left = P;
								Q = P;
								temp = P;
							}
							else if (P_par->right == P) {
								P = P->left;
								P_par->right = P;
								Q = P;
								temp = P;
							}
						}
							
					}
					else if (P->right != nullptr) {
						if (root->right == P) {
							P = P-> right;
							root->right = P;
						}
						else {
							//P_par = P;
							/*nodes_stack.push_back(P);*/
							if (P_par->left == P) {
								P = P->right;
								P_par->left = P;
								Q = P;
								temp = P;
							}
							else if (P_par->right == P) {
								P = P->right;
								P_par->right = P;
								Q = P;
								temp = P;
							}
						}
							
					}

			}
		}
			virtual void __remove_hook(typename ::AC_Node<T1,T2> * &root) {
				// REBALANCING
				// если P - лист, то пойдем до корня перебалансировать
				//P - deleted note
				// temp - Node from we come when balance tree
				//temp_par - 
				//Q - Node for switching
				//Z - temp node for switching

				if (!nodes_stack.empty()) {
					if (nodes_stack.back() == P) {
						if (P->right == nullptr and P->bf.avl == 0) {
							P->bf.avl = -1;
							nodes_stack.clear();
							return;
						}
						else if (P->right == nullptr and P->bf.avl == -1) {

							Q = P->left;
							if (Q->bf.avl == 0) {
								a = 1;
							}
							if (Q->bf.avl == -1) {
								a = 0;
							}

							P->left = Q->right;
							/*temp_par->bf.avl +=1;*/
							Q->right = P;

							if (a == 1) {
								Q->bf.avl = 1;
								P->bf.avl = -1;
							}
							else if (a == 0) {
								Q->bf.avl = 0;
								P->bf.avl = 0;
							}
							

							nodes_stack.pop_back();
							temp_par = nodes_stack.back();
							if (temp_par->left == P) {
								temp_par->left = Q;
							}
							else if (temp_par->right == P) {
								temp_par->right = Q;
							}
							nodes_stack.clear();
							return;

						}
						else if (P->left == nullptr and P->bf.avl == 0) {
							P->bf.avl = 1;
							nodes_stack.clear();
							return;
						}
						else if (P->left == nullptr and P->bf.avl == 1) {
							Q = P->right; //Q = b
							if (Q->bf.avl == 0) {
								a = 0;
							}
							else if (Q->bf.avl == 1) {
								a = 1;
							}
							P->right = Q->left;
							Q->left = P;
							if (a == 1) {
								Q->bf.avl = 0;
								P->bf.avl = 0;
							}
							else if (a == 0) {
								Q->bf.avl = -1;
								P->bf.avl = 1;
							}	


							nodes_stack.pop_back();
							temp_par = nodes_stack.back();
							if (temp_par->left == P) {
								temp_par->left = Q;
							}
							else if (temp_par->right == P) {
								temp_par->right = Q;
							}
							nodes_stack.clear();
							return;
						}
					}
				}
				
				if (P->left == nullptr and P->right == nullptr) {
					while(!nodes_stack.empty()){
						if (temp_par != nullptr) { // 1st pop from stack check
							if (nodes_stack.back()->right == temp_par) {
								
								if (temp == Q and P!= Q) { // if Q not change we dont connect it
									//если мы зашли сюда, то поворот был
									nodes_stack.back()->right = Q;
									temp = Q;
									Q = nullptr;
								}
								else {
									temp = temp_par; // перекидываем temp для расстановки балансов
								}
								
							}
							else if (nodes_stack.back()->left == temp_par) {
								if (Q != P) {
									nodes_stack.back()->left = Q;
									temp = Q;
									Q = nullptr;
								}
								else {
									temp = temp_par;
								}
							}
						} 
						// pop parent from stack
						temp_par = nodes_stack.back();
						nodes_stack.pop_back();
						S = temp_par;

						// list check and rebalancing
						if (temp_par->left == nullptr and temp_par->right == nullptr) {
							temp_par->bf.avl = 0;
							continue;
						}
						//come from leftside
						if (temp_par->left == temp) {
							if (temp_par->bf.avl == -1) {
								temp_par->bf.avl -= 1;
							}
							else if (temp_par->bf.avl == 0) {
								temp_par->bf.avl = 1;
								return;
							}
							else {
								temp_par->bf.avl += 1;
							}
							/*temp_par->bf.avl += 1;
							temp = temp_par;*/
						}
						//come from rightside
						else if (temp_par->right == temp) {
							if(temp_par->bf.avl == 1){
								temp_par->bf.avl -= 1; 
							}
							else if(temp_par->bf.avl == 0){
								temp_par->bf.avl = -1;
								return;
							}
							else{ 
								temp_par->bf.avl -= 1;
							}
							/*temp_par->bf.avl -= 1;
							temp = temp_par;*/
						}


						// SWITCHES
						if (temp_par->bf.avl > 1 or temp_par->bf.avl < -1) {
							if (temp_par->bf.avl == 2) { //LEFT
								if (temp_par->right->bf.avl >= 0) { // zig
									Q = temp_par->right; //Q = b
									if (Q->bf.avl == 0) {
										a = 0;
									}
									else if (Q->bf.avl == 1) {
										a = 1;
									}
									temp_par->right = Q->left;
									Q->left = temp_par;

									// rebalancing

									if (a == 1) {
										Q->bf.avl = 0;
										temp_par->bf.avl = 0;
									}
									if (a == 0) {
										Q->bf.avl = -1;
										temp_par->bf.avl = 1;
									}
								}
								else if (temp_par->right->bf.avl < 1) { // zig zag
									Z = temp_par->right;
									Q = Z->left;
									if (Q->bf.avl == 0) {
										a = 0;
									}
									else if (Q->bf.avl == 1) {
										a = 1;
									}
									else if (Q->bf.avl == -1) {
										a = -1;
									}

									if (Q->right != nullptr) {
										Z->left = Q->right;
									}
									else {
										Z->left = nullptr;
									}
									if (Q->left != nullptr) {
										temp_par->right = Q->left;
									}
									else {
										temp_par->right = nullptr;
									}
									Q->right = Z;
									Q->left = temp_par;
									Q->bf.avl = 0;
									//rebalancing
									if (a == 0) {
										Q->left->bf.avl = 0;
										Q->right->bf.avl = 0;
									}
									else if (a == 1) {
										Q->left->bf.avl = -1;
										Q->right->bf.avl = 0;
									}
									else if (a == -1) {
										Q->left->bf.avl = 0;
										Q->right->bf.avl = 1;
									}


								}
							}
							else if (temp_par->bf.avl == -2) { // RIGHT
								if (temp_par->left->bf.avl < 1) { // zig

									Q = temp_par->left;
									if (Q->bf.avl == 0) {
										a = 1;
									}
									if (Q->bf.avl == -1) {
										a = 0;
									}

									temp_par->left = Q->right;
									/*temp_par->bf.avl +=1;*/
									Q->right = temp_par;

									if (a == 1) {
										Q->bf.avl = 1;
										temp_par->bf.avl = -1;
									}
									else if (a == 0) {
										Q->bf.avl = 0;
										temp_par->bf.avl = 0;
									}
									// херня здесь

								}
								else if (temp_par->left->bf.avl == 1) { // zig zag
									Z = temp_par->left;
									Q = Z->right;
									if (Q->bf.avl == 0) {
										a = 0;
									}
									else if (Q->bf.avl == 1) {
										a = 1;
									}
									else if (Q->bf.avl == -1) {
										a = -1;
									}
									Z->right = Q->left;
									temp_par->left = Q->right;
									Q->left = Z;
									Q->right = temp_par;

									//rebalancing
									Q->bf.avl = 0;
									if (a == 0) {

										Q->left->bf.avl = 0;
										Q->right->bf.avl = 0;
									}
									else if (a == 1) {

										Q->right->bf.avl = 0;
										Q->left->bf.avl = -1;
									}
									else if (a == -1) {

										Q->left->bf.avl = 1;
										Q->right->bf.avl = 0;
									}
								}
							}
							if (Q->bf.avl != 0) {
								if (nodes_stack.back()->right == temp_par) {

									if (Q != P) { // if Q not change we dont connect it
										nodes_stack.back()->right = Q;
										temp = Q;
									}
									else {
										temp = temp_par; // перекидываем temp для расстановки балансов
									}

								}
								else if (nodes_stack.back()->left == temp_par) {
									if (Q != P) {
										nodes_stack.back()->left = Q;
										temp = Q;
									}
									else {
										temp = temp_par;
									}
								}
								break;
							}
							temp = Q;
						}
					}
				}
				else {


					//тащим из стека пока не дойдем до Р и меняем баланс
					while (nodes_stack.back() != P) {
						if (temp_par != nullptr) { // 1st pop from stack check
							if (nodes_stack.back()->right == temp_par) {

								if (Q != P and temp == Q) { // if Q not change we dont connect it
									nodes_stack.back()->right = Q;
									temp = Q;
									Q = nullptr;
								}
								else {
									temp = temp_par; // перекидываем temp для расстановки балансов
								}

							}
							else if (nodes_stack.back()->left == temp_par) {
								if (Q != P) { // if Q not change we dont connect it
									nodes_stack.back()->left = Q;
									temp = Q;
									Q = nullptr;
								}
								else {
									temp = temp_par;
								}
							}
						}

						temp_par = nodes_stack.back();
						nodes_stack.pop_back();

						if (temp_par->left == temp) {
							if (temp_par->bf.avl == -1) {
								temp_par->bf.avl = 0;
							}
							else if (temp_par->bf.avl == 0) {
								temp_par->bf.avl = 1;
								return;
							}
							else {
								temp_par->bf.avl += 1;
							}
							/*temp_par->bf.avl += 1;
							temp = temp_par;*/
						}


						else if (temp_par->right == temp) {
							if (temp_par->bf.avl == 1) {
								temp_par->bf.avl = 0;
							}
							else if (temp_par->bf.avl == 0) {
								temp_par->bf.avl = -1;
								return;
							}
							else{ temp_par->bf.avl -= 1; }
							/*temp_par->bf.avl -= 1;
							temp = temp_par;*/
						}


						if (temp_par->bf.avl < -1 or temp_par->bf.avl > 1) {
							if (temp_par->bf.avl == 2) { //LEFT
								if (temp_par->right->bf.avl == 1) { // zig
									Q = temp_par->right; //Q = b
									temp_par->right = Q->left;
									Q->left = temp_par;
									Q->bf.avl = 0;
									Q->left->bf.avl = 0;
									temp_par->bf.avl = 0;
								}
								else if (temp_par->right->bf.avl < 1) { // zig zag
									Z = temp_par->right;
									Q = Z->left;
									if (Q->right != nullptr) {
										Z->left = Q->right;
									}
									else {
										Z->left = nullptr;
									}
									if (Q->left != nullptr) {
										temp_par->right = Q->left;
									}
									else {
										temp_par->right = nullptr;
									}
									Q->right = Z;
									Q->left = temp_par;
								}
							}
							else if (temp_par->bf.avl == -2) { // RIGHT
								if (temp_par->left->bf.avl < 1) { // zig
									Q = temp_par->left;
									temp_par->left = Q->right;
									Q->right = temp_par;
								}
								else if (temp_par->left->bf.avl == 1) { // zig zag
									Z = temp_par->left;
									Q = Z->right;
									Z->right = Q->left;
									temp_par->left = Q->right;
									Q->left = Z;
									Q->right = temp_par;
								}
							}
							if (Q != P) {
								if (Q->bf.avl != 0) {
									if (nodes_stack.back()->right == temp_par) {

										if (Q != P) { // if Q not change we dont connect it
											nodes_stack.back()->right = Q;
											temp = Q;
										}
										else {
											temp = temp_par; // перекидываем temp для расстановки балансов
										}

									}
									else if (nodes_stack.back()->left == temp_par) {
										if (Q != P) {
											nodes_stack.back()->left = Q;
											temp = Q;
										}
										else {
											temp = temp_par;
										}
									}
									break;
								}
							}
							temp = Q;
						}
					}
				}
				// если стек пустой, то корень меняется
				if (nodes_stack.empty()) {
					if (Q != nullptr) {
						if (temp_par == Q->right or temp_par == Q->left) {
							root->right = Q;
						}
					}
					/*if (temp_par == root->right) {
						root->right = Q;
					}*/
				}
				nodes_stack.clear();
			}
	};


	class avl_insert : public BST<T1, T2>::Insert {
		typename ::AC_Node<T1, T2>* T; // parent P
		typename ::AC_Node<T1, T2>* S; // node with risk of rebalancing
		typename ::AC_Node<T1, T2>* P /*AVL<T1, T2>::head*/; // temp node for search
		typename ::AC_Node<T1, T2>* Q /*= AVL<T1, T2>::head*/; // node - son of P (left or right)
		typename ::AC_Node<T1, T2>* R;
		typename ::AC_Node<T1,T2>* Z;
	public:
		int a = 0;
		Comparator<T1>insert_cmp;
		
		friend AVL::AVL(Comparator<T1>& cmp);
		friend void AVL::Insert_template(AC_Node<T1, T2>*& root, T1 key,T2 value);
		avl_insert(Comparator<T1>& cmp) : Insert(cmp) {}

	protected:
		virtual void __insert(typename ::AC_Node<T1,T2>*& root, T1 key, T2 value) override {
			if (root == nullptr) {
				root = new typename ::AC_Node<T1,T2>;
				root->right = nullptr;
				root->left = nullptr;
				root->bf.avl = 0;
			}
			T = root;
			P = root->right;
			S = root->right;
			Q = P;
			if (root->right == nullptr) {
				root->right = new typename ::AC_Node<T1,T2>;
				root->right->key = key;
				root->right->value = value;
				root->right->left = nullptr;
				root->right->right = nullptr;
				root->right->bf.avl = 0;

			}

			else {
				while (typename BST<T1, T2>::Insert::cmp_in_btp.compare(P->key, key) != 0) {
					if (typename BST<T1, T2>::Insert::cmp_in_btp.compare(P->key, key) > 0) { //go right
						Q = P->right; // А4
						if (Q == nullptr) {
							Q = new typename ::AC_Node<T1,T2>;
							Q->key = key;
							Q->value = value;
							Q->left = nullptr;
							Q->right = nullptr;
							Q->bf.avl = 0;
							P->right = Q;
							break;

						}
						else {
							if (Q->bf.avl != 0) {
								T = P;
								S = Q;
							}

						}
						P = Q;
						continue;
					}
					if (typename BST<T1, T2>::Insert::cmp_in_btp.compare(P->key, key) < 0) { //go left
						Q = P->left; // А3
						if (Q == nullptr) {
							Q = new typename ::AC_Node<T1,T2>;
							Q->key = key;
							Q->value = value;
							Q->left = nullptr;
							Q->right = nullptr;
							Q->bf.avl = 0;
							P->left = Q;
							break;
						}
						else {
							if (Q->bf.avl != 0) {
								T = P;
								S = Q;
							}

						}
						P = Q;
						continue;

					}
				}
				if (typename BST<T1, T2>::Insert::cmp_in_btp.compare(key, (S)->key) > 0) { // корректировка фб

					a = -1; // параметр, куда добавлен узел 
				}
				else {

					a = 1;
				}
				if (a == 1) {
					P = S->right;
					R = P;
				}
				else if (a == -1) {
					P = S->left;
					R = P;
				}
				while (P != Q) {
					if (typename BST<T1, T2>::Insert::cmp_in_btp.compare(key, P->key) > 0) {

						P->bf.avl = -1;
						P = P->left;
					}
					if (typename BST<T1, T2>::Insert::cmp_in_btp.compare(key, P->key) < 0) {
						P->bf.avl = 1;
						P = P->right;
					}
				}

			}
				
		}
		virtual void __insert_hook(typename ::AC_Node<T1,T2>*& root) override { // balancing
			if (S == nullptr) {
				return;
			}
			if (S->bf.avl == 0) {
				S->bf.avl = a;
				root->bf.avl = root->bf.avl + 1;
				return;
			}
			if (S->bf.avl == -a) { // более сблансированное
				S->bf.avl = 0;
				return;
			}
			if (S->bf.avl == a) {
				if (R->bf.avl == a) {  // A8 ZIG
					P = R;
					if (a == 1) {
						S->right = R->left;
						R->left = S;
						S->bf.avl = 0;
						R->bf.avl = 0;
					}
					if (a == -1) {
						S->left = R->right;
						R->right = S; //R->S->R
						S->bf.avl = 0;
						R->bf.avl = 0;

					}

				}
				else if (R->bf.avl == -a) { // ZIG ZAG A9
					if (a == 1) {
						P = R->left;
						R->left = P->right;
						P->right = R;
						S->right = P->left;
						P->left = S;
					}
					if (a == -1) {
						P = R->right;
						R->right = P->left;
						P->left = R;
						S->left = P->right;
						P->right = S;
					}
					if (P->bf.avl == a) {
						S->bf.avl = -a;
						R->bf.avl = 0;
					}
					if (P->bf.avl == 0) {
						S->bf.avl = 0;
						R->bf.avl = 0;
					}
					if (P->bf.avl == -a) {
						S->bf.avl = 0;
						R->bf.avl = a;
					}
					P->bf.avl = 0;
				}
				if (S == T->right) {
					T->right = P;
				}
				else {
					T->left = P; // ERROR
				}
			}

		}
	};

	avl_remove* avl_rmv;
	avl_insert* avl_insrt;

	virtual void Remove_template(AC_Node<T1, T2>*& root, T1 key) override {
		avl_rmv->nodes_stack.clear();
		avl_rmv->__remove(BST<T1,T2>::root, key);
		avl_rmv->__remove_hook(BST<T1, T2>::root);
	}

	virtual void Insert_template(AC_Node<T1, T2>*& root, T1 key, T2 value) override{
		avl_insrt->__insert(BST<T1, T2>::root, key, value);
		avl_insrt->__insert_hook(BST<T1, T2>::root);
	}

		/*class avl_search : public BST<T1, T2>::Search {
			friend void AVL::search(T1 key);
		};*/

		/*virtual void Remove_template(AC_Node<T1, T2>*& root, T1 key) override {
			rmv->__remove(root, key);
		}*/

		/*virtual void Insert_template(AC_Node<T1, T2>*& root, T1 key, T2 value) {}

		virtual AC_Node<T1, T2>* Search_template(AC_Node<T1, T2>* root, T1 key) {}*/


	public:
		virtual AC_Node<T1,T2>* search(T1 key) override {
			return BST<T1,T2>::Search_template(BST<T1, T2>::root->right, key);
		}


		//virtual void insert(T1 key, T2 value) override {
		//	Insert_template()
		//	/*avl_insert insrt(cmp_in_avl);
		//	insrt.__insert(BST<T1, T2>::root, key, value);
		//	insrt.__insert_hook(BST<T1, T2>::root);*/
		//}

		//virtual void remove(T1 key) override {
		//	avl_remove avl_rmv(cmp_in_avl);
		//	avl_rmv.__remove(BST<T1, T2>::root, key);
		//	avl_rmv.__remove_hook(BST<T1, T2>::root);
		//}
		virtual void prefix(bool (*cb_ptr)(T1 key, T2 value, int depth,int bf))override {
			if (BST<T1,T2>::root->right == nullptr) {
				cout << "tree is empty." << endl;
				return;
			}
			BST<T1, T2>::__prefix(BST<T1, T2>::root->right, cb_ptr);
		}
	

 };