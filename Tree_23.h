#pragma once

using namespace std;

template <class T1,class T2>



class Tree_23 : public AC<T1, T2> {
protected:
	Comparator<T1> cmp_in_23;
	Node_23<T1, T2>* root;
	int depth_for_traverse;


public:


	// CONSTRUCT
	Tree_23(Comparator<T1>& cmp) {
		root = nullptr;
		cmp_in_23 = cmp;
		insrt_in_23 = new Insert(cmp_in_23);
		srch_in_23 = new Search(cmp_in_23);
		rmv_in_23 = new Remove(cmp_in_23);
	}



	// DECONSTRUCT
	~Tree_23() {
		delete insrt_in_23;
		delete srch_in_23;
		delete rmv_in_23;
	}

	virtual void prefix(bool (*cb_ptr)(pair<T1, T2>* pairs, int depth)) {
		__prefix(root, cb_ptr);
	}

	virtual void insert(T1 key, T2 value) override {
		Insert_template(root, key, value);
	}
	virtual AC_Node<T1, T2>* search(T1 key) override {
		return nullptr;
	}

	virtual Node_23<T1, T2>* searchhh(T1 key) {
		return srch_in_23->__search(root, key);
	}
	virtual void remove(T1 key) override {
		rmv_in_23->__remove(root, key);
	}

protected:
	class Search {

		friend AC_Node<T1, T2>* Tree_23::search(T1 key);
		friend Node_23<T1, T2>* Tree_23::searchhh(T1 key);
		friend Tree_23<T1, T2>::Tree_23(Comparator<T1>& cmp);
		//friend void Tree_23<T1,T2>::Remove::__remove(Node_23<T1, T2>* node,T1 key);
		Search(Comparator<T1>& cmp) {
			cmp_in_23 = cmp;
		}
	private:
		Comparator<T1> cmp_in_23;
		bool is_leaf(Node_23<T1, T2>* node) { // явл€етс€ ли узел листом; проверка используетс€ при вставке и удалении.
			return (node->first == nullptr) && (node->second == nullptr) && (node->third == nullptr);
		}
		bool find(Node_23<T1, T2>* node, T1 key) {
			for (int i = 0; i < node->keysize; i++) {
				if (cmp_in_23.compare(node->kv[i].first, key) == 0) {
					return true;
				}
			}
			return false;
		}

		virtual Node_23<T1, T2>* __search(Node_23<T1, T2>* root, T1 key) {
			if (root == nullptr) {
				throw search_exc("23 SEARCH EXCEPTION : this element doesnt exist.");
			}

			if (find(root, key)) {
				return root;
			}
			else if (cmp_in_23.compare(root->kv[0].first, key) < 0) {
				return __search(root->first, key);
			}
			else if (root->keysize == 1 or (root->keysize == 2 and cmp_in_23.compare(root->kv[1].first, key) < 0)) {
				return __search(root->second, key);
			}
			else if (root->keysize == 2) {
				return __search(root->third, key);
			}

		}
		virtual Node_23<T1, T2>* __search_hook(Node_23<T1, T2>* root) {
			return nullptr;
		}
	};
	class Insert {
		friend void Tree_23::Insert_template(Node_23<T1, T2>*& root, T1 key, T2 value);
		friend Tree_23::Tree_23(Comparator<T1>& cmp);


		Insert(Comparator<T1>& cmp) {
			cmp_in_23 = cmp;
		}
	private:
		Comparator<T1> cmp_in_23;
		Node_23<T1, T2>* searcher;
		Node_23<T1, T2>* result;
		Node_23<T1, T2>* x_parent;
		Node_23<T1, T2>* y_parent;
		Node_23<T1, T2>* parent_of_node; // parent in split method
		Node_23<T1, T2>* node_in_split_parent;
		Node_23<T1, T2>* parent_from_method;
		list<Node_23<T1, T2>*> nodes_stack;

		/*Node_23<T1, T2>* parent(Node_23<T1, T2>* node){
			if (nodes_stack.empty()) {
				return nullptr;
			}
			parent_from_method = nodes_stack.back();
			nodes_stack.pop_back();
			return parent_from_method;
		}*/
	public:
		void swap(pair<T1, T2>& p1, pair<T1, T2>& p2) {
			T1 temp_k = p1.first;
			T2 temp_v = p1.second;
			p1 = p2;
			p2 = make_pair(temp_k, temp_v);
		}

		void sort2(pair<T1, T2>& p1, pair<T1, T2>& p2) {
			if (cmp_in_23.compare(p1.first, p2.first) < 0) {
				swap(p1, p2);
			}
		}

		void sort3(pair<T1, T2>& p1, pair<T1, T2>& p2, pair<T1, T2>& p3) {
			if (cmp_in_23.compare(p1.first, p2.first) < 0) {
				swap(p1, p2);
			}
			if (cmp_in_23.compare(p1.first, p3.first) < 0) {
				swap(p1, p3);
			}
			if (cmp_in_23.compare(p2.first, p3.first) < 0) {
				swap(p2, p3);
			}
		}

		void sort(Node_23<T1, T2>*& node) { //  лючи в вершинах должны быть отсортированы
			if (node->keysize == 1) {
				return;
			}
			if (node->keysize == 2) {
				sort2(node->kv[0], node->kv[1]);
			}
			if (node->keysize == 3) {
				sort3(node->kv[0], node->kv[1], node->kv[2]);
			}
		}

		void insert_to_node(Node_23<T1, T2>*& node, pair<T1, T2> pr) {
			node->kv[node->keysize] = pr;
			node->keysize++;
			sort(node);
		}

		bool is_leaf(Node_23<T1, T2>* node) { // явл€етс€ ли узел листом; проверка используетс€ при вставке и удалении.
			return (node->first == nullptr) && (node->second == nullptr) && (node->third == nullptr);
		}

		void become_2(T1 key, T2 value, Node_23<T1, T2>*& src, Node_23<T1, T2>* node1, Node_23<T1, T2>* node2) {
			src->kv[0] = make_pair(key, value);
			src->kv[1] = make_pair(0, nullptr);
			src->kv[2] = make_pair(0, nullptr);
			src->first = node1;
			src->second = node2;
			src->third = nullptr;
			src->fourth = nullptr;
			src->parent = nullptr;
			src->keysize = 1;

		}

		//void parent(Node_23<T1,T2>*node,Node_23<T1,T2>*root){
		//	Node_23<T1, T2>* searcher = root;

		//	//TREE IS EMPTY
		//	if (searcher == nullptr) {
		//		return nullptr;
		//	}

		//	// IF ONLY 1 NODE
		//	if (is_leaf(searcher)) {
		//		return nullptr;
		//	}

		//	while (searcher->kv[0].first != node->kv[0].first) {
		//		if (searcher == nullptr) {
		//			throw insert_exc("23 INSERT EXCEPTION (parent) : cant find this node...");
		//		}

		//		if (searcher->first == node or searcher->second == node or searcher->third == node) {
		//			return searcher;
		//		}

		//		if (cmp_in_23.compare(searcher->kv[0].first, node->kv[0].first) < 0) {
		//			searcher = searcher->first;
		//			continue;
		//		}

		//		if (searcher->keysize > 1) {
		//			if (cmp_in_23.compare(searcher->kv[0].first, node->kv[0].first) > 0 \
		//				and \
		//				cmp_in_23.compare(searcher->kv[1].first,node->kv[0].first) < 0) {
		//				searcher = searcher->second;
		//				continue;
		//			}
		//			if (searcher->keysize == 2) {
		//				if (cmp_in_23.compare(searcher->kv[1].first, node->kv[0].first) > 0) {
		//					searcher = searcher->third;
		//					continue;
		//				}
		//			}
		//			/*if (searcher->keysize == 3) {
		//				if(cmp_in_23.compare(searcher->kv[2].first, node->kv[0].first) > 0)
		//			}*/
		//		}
		//	}
		//}



		virtual Node_23<T1, T2>* __insert(Node_23<T1, T2>*& root, T1 key, T2 value) {
			if (root == nullptr) {
				root = new Node_23<T1, T2>;
				root->keysize = 0;
				root->first = nullptr;
				root->second = nullptr;
				root->third = nullptr;
				root->fourth = nullptr;
				root->parent = nullptr;
			}

			// если дерево пусто, то создаем первую 2-3-вершину (корень)

			if ((cmp_in_23.compare(root->kv[0].first, key) == 0 or cmp_in_23.compare(root->kv[1].first, key) == 0 or \
				cmp_in_23.compare(root->kv[2].first, key) == 0) and (root->kv[0].first != 0)) {
				throw insert_exc("23 INSERT EXCEPTION: this element already exist...");
			}


			// WORK VERS
			if (is_leaf(root)) {
				if (!nodes_stack.empty()) {
					parent_of_node = nodes_stack.back();
					nodes_stack.pop_back();
				}
				else {
					parent_of_node = nullptr;
				}
				insert_to_node(root, make_pair(key, value));
			}
			else if (cmp_in_23.compare(root->kv[0].first, key) < 0) {
				nodes_stack.push_back(root);
				__insert(root->first, key, value);
				if (!nodes_stack.empty()) {
					parent_of_node = nodes_stack.back();
					nodes_stack.pop_back();
				}
				else {
					parent_of_node = nullptr;
				}
			}
			else if ((root->keysize == 1) or ((root->keysize == 2) and (cmp_in_23.compare(root->kv[1].first, key) < 0))) {
				nodes_stack.push_back(root);
				__insert(root->second, key, value);
				if (!nodes_stack.empty()) {
					parent_of_node = nodes_stack.back();
					nodes_stack.pop_back();
				}
				else {
					parent_of_node = nullptr;
				}
			}
			else {
				nodes_stack.push_back(root);
				__insert(root->third, key, value);
				if (!nodes_stack.empty()) {
					parent_of_node = nodes_stack.back();
					nodes_stack.pop_back();
				}
				else {
					parent_of_node = nullptr;
				}
			}

			return split(root, parent_of_node);
		}

		virtual Node_23<T1, T2>* split(Node_23<T1, T2>* node, Node_23<T1, T2>* parent_of_node) {

			if (node == parent_of_node) {
				parent_of_node = nullptr;
			}
			if (node->keysize < 3) {
				return node;
			}




			// MAKE FIELDS FOR XY PARENTS




			// create 2 new nodes from 3-node

			Node_23<T1, T2>* x = new Node_23<T1, T2>;
			x->keysize = 1;
			x->kv[0] = node->kv[0];
			x->first = node->first;
			x->second = node->second;
			x->third = nullptr;
			x->fourth = nullptr;
			x_parent = parent_of_node;

			Node_23<T1, T2>* y = new Node_23<T1, T2>;
			y->keysize = 1;
			y->kv[0] = node->kv[2];
			y->first = node->third;
			y->second = node->fourth;
			y->third = nullptr;
			y->fourth = nullptr;
			y_parent = parent_of_node;



			//if (x->first) {
			//	x->first->parent = x;    // ѕравильно устанавливаем "родител€" "сыновей".
			//}
			//if (x->second) {
			//	x->second->parent = x;   // ѕосле разделени€, "родителем" "сыновей" €вл€етс€ "дедушка",
			//}
			//if (y->first) {
			//	y->first->parent = y;    // ѕоэтому нужно правильно установить указатели.
			//}
			//if (y->second) {
			//	y->second->parent = y;
			//}

			if (parent_of_node) {
				insert_to_node(parent_of_node, node->kv[1]);

				if (parent_of_node->first == node) {
					parent_of_node->first = nullptr;
				}
				else if (parent_of_node->second == node) {
					parent_of_node->second = nullptr;
				}
				else if (parent_of_node->third == node) {
					parent_of_node->third = nullptr;
				}


				if (parent_of_node->first == nullptr) {
					parent_of_node->fourth = parent_of_node->third;
					parent_of_node->third = parent_of_node->second;
					parent_of_node->second = y;
					parent_of_node->first = x;
				}
				else if (parent_of_node->second == nullptr) {
					parent_of_node->fourth = parent_of_node->third;
					parent_of_node->third = y;
					parent_of_node->second = x;
				}
				else {
					parent_of_node->fourth = y;
					parent_of_node->third = x;
				}


				Node_23<T1, T2>* tmp = parent_of_node;
				node = nullptr;
				return tmp;

			}
			else {
				// “ак как в эту ветку попадает только корень, делаем родителем дел€щийс€ элемент
				x_parent = node;
				y_parent = node;
				become_2(node->kv[1].first, node->kv[1].second, node, x, y);
				parent_of_node = nullptr;
				return node;
			}

		}


	};

	class Remove {
		friend Tree_23::Tree_23(Comparator<T1>& cmp);
		friend virtual void Tree_23::remove(T1 key);

		Remove(Comparator<T1>& cmp) {
			cmp_in_23 = cmp;
		}
	private:
		Comparator<T1>cmp_in_23;
		Node_23<T1, T2>* parent_of_node_in_fix = nullptr;
		Node_23<T1, T2>* next_node_for_fix;
		list<Node_23<T1, T2>*>nodes_stack;
		Node_23<T1, T2>* parent_for_min;


		void swap(pair<T1, T2>& p1, pair<T1, T2>& p2) {
			T1 temp_k = p1.first;
			T2 temp_v = p1.second;
			p1 = p2;
			p2 = make_pair(temp_k, temp_v);
		}
		void sort2(pair<T1, T2>& p1, pair<T1, T2>& p2) {
			if (cmp_in_23.compare(p1.first, p2.first) < 0) {
				swap(p1, p2);
			}
		}
		void sort3(pair<T1, T2>& p1, pair<T1, T2>& p2, pair<T1, T2>& p3) {
			if (cmp_in_23.compare(p1.first, p2.first) < 0) {
				swap(p1, p2);
			}
			if (cmp_in_23.compare(p1.first, p3.first) < 0) {
				swap(p1, p3);
			}
			if (cmp_in_23.compare(p2.first, p3.first) < 0) {
				swap(p2, p3);
			}
		}
		void sort(Node_23<T1, T2>*& node) { //  лючи в вершинах должны быть отсортированы
			if (node->keysize == 1) {
				return;
			}
			if (node->keysize == 2) {
				sort2(node->kv[0], node->kv[1]);
			}
			if (node->keysize == 3) {
				sort3(node->kv[0], node->kv[1], node->kv[2]);
			}
		}
		Node_23<T1, T2>* __search(Node_23<T1, T2>* root, T1 key) {
			if (root == nullptr) {
				throw search_exc("23 SEARCH EXCEPTION : this element doesnt exist.");
			}

			if (find(root, key)) {
				if (!nodes_stack.empty()) {
					nodes_stack.pop_back();
				}
				return root;
			}
			else if (cmp_in_23.compare(root->kv[0].first, key) < 0) {
				nodes_stack.push_back(root);
				parent_of_node_in_fix = root;
				return __search(root->first, key);
			}
			else if (root->keysize == 1 or (root->keysize == 2 and cmp_in_23.compare(root->kv[1].first, key) < 0)) {
				nodes_stack.push_back(root);
				parent_of_node_in_fix = root;
				return __search(root->second, key);
			}
			else if (root->keysize == 2) {
				nodes_stack.push_back(root);
				parent_of_node_in_fix = root;
				return __search(root->third, key);
			}

		}
		void insert_to_node(Node_23<T1, T2>*& node, pair<T1, T2> pr) {
			node->kv[node->keysize] = pr;
			node->keysize++;
			sort(node);
		}
		void remove_from_node(Node_23<T1, T2>* node, pair<T1, T2>& kv) {
			if (node->keysize >= 1 and cmp_in_23.compare(node->kv[0].first, kv.first) == 0) {
				node->kv[0] = node->kv[1];
				node->kv[1] = node->kv[2];
				node->keysize--;
			}
			else if (node->keysize == 2 and cmp_in_23.compare(node->kv[1].first, kv.first) == 0) {
				node->kv[1] = node->kv[2];
				node->keysize--;
			}
		}
		bool find(Node_23<T1, T2>* node, T1 key) {
			for (int i = 0; i < node->keysize; i++) {
				if (cmp_in_23.compare(node->kv[i].first, key) == 0) {
					return true;
				}
			}
			return false;
		}
		Node_23<T1, T2>* __search_min(Node_23<T1, T2>* node) {
			if (node == nullptr) {
				return nullptr;
			}
			if (node->first == nullptr) {
				return node;
			}
			else {
				nodes_stack.push_back(node);
				parent_for_min = node;
				return __search_min(node->first);
			}
		}
		pair<T1, T2> pair_for_remove(Node_23<T1, T2>* node, T1 key) {
			if (cmp_in_23.compare(node->kv[0].first, key) == 0) {
				return node->kv[0];
			}
			else if (node->keysize == 2 and cmp_in_23.compare(node->kv[1].first, key) == 0) {
				return node->kv[1];
			}
			else if (node->keysize == 3 and cmp_in_23.compare(node->kv[2].first, key) == 0) {
				return node->kv[2];
			}
		}
		Node_23<T1, T2>* parent() {
			Node_23<T1, T2>* res = nullptr;
			if (!nodes_stack.empty()) {
				res = nodes_stack.back();
				nodes_stack.pop_back();
			}
			return res;
		}

		void __remove(Node_23<T1, T2>*& root, T1 key) {
			Node_23<T1, T2>* item = __search(root, key);
			pair<T1, T2> remove_pair = pair_for_remove(item, key);
			Node_23<T1, T2>* min = nullptr;
			if (item == nullptr) {
				throw remove_exc("cant delete");
			}

			if (cmp_in_23.compare(item->kv[0].first, key) == 0) {
				/*nodes_stack.push_back(item);*/
				parent_for_min = item;
				min = __search_min(item->second);
			}

			else {
				parent_for_min = item;
				min = __search_min(item->third);
			}

			if (min) {
				pair<T1, T2>& pair_for_swap = (cmp_in_23.compare(item->kv[0].first, key) == 0 ? item->kv[0] : item->kv[1]);
				swap(pair_for_swap, min->kv[0]);

				Node_23<T1, T2>* temp_par = parent_of_node_in_fix;
				parent_of_node_in_fix = parent();
				item = min;
				if (temp_par != nullptr) {
					nodes_stack.push_back(temp_par);
				}
				parent_of_node_in_fix = parent_for_min;
			}

			remove_from_node(item, remove_pair);
			fix(item,root);
			return;
		}



		Node_23<T1, T2>* fix(Node_23<T1, T2>* node,Node_23<T1,T2>*&root) {

			// ONLY 1 NODE IN TREE
			if (node->keysize == 0 and parent_of_node_in_fix == nullptr) {
				node = nullptr;
				return nullptr;
			}

			if (node->keysize != 0) {
				if (parent_of_node_in_fix != nullptr) {
					next_node_for_fix = parent_of_node_in_fix;
					parent_of_node_in_fix = parent();
					if (root->first == next_node_for_fix or root->second == next_node_for_fix or root->third == next_node_for_fix) {
						parent_of_node_in_fix = root;
					}
					return fix(next_node_for_fix,root);
				}
				else {
					root = node;
					return root;
				}
			}

			//2 : REDISTRIBUTE
			Node_23<T1, T2>* one_more_parent = parent_of_node_in_fix;
			if (one_more_parent != nullptr) {
				if (one_more_parent->first->keysize == 2 or one_more_parent->second->keysize == 2 or one_more_parent->keysize == 2) {
					node = redistribute(node);
				}

				else if (one_more_parent->keysize == 2 and one_more_parent->third->keysize == 2) {
					node = redistribute(node);
				}

				else {
					node = merge(node,root);
				}
			}

			return fix(node,root);


		}

		Node_23<T1, T2>* redistribute(Node_23<T1, T2>* node) {
			Node_23<T1, T2>* son1 = parent_of_node_in_fix->first;
			Node_23<T1, T2>* son2 = parent_of_node_in_fix->second;
			Node_23<T1, T2>* son3 = parent_of_node_in_fix->third;

			if (parent_of_node_in_fix->keysize == 2 and son1->keysize < 2 and son2->keysize < 2 and son3->keysize < 2) {
				if (son1 == node) {
					parent_of_node_in_fix->first = parent_of_node_in_fix->second;
					parent_of_node_in_fix->second = parent_of_node_in_fix->third;
					parent_of_node_in_fix->third = nullptr;
					insert_to_node(parent_of_node_in_fix->first, parent_of_node_in_fix->kv[0]); // ???
					parent_of_node_in_fix->first->third = parent_of_node_in_fix->first->second;
					parent_of_node_in_fix->first->second = parent_of_node_in_fix->first->first;

					if (node->first != nullptr) {
						parent_of_node_in_fix->first->first = node->first;
					}
					else if (node->second != nullptr) {
						parent_of_node_in_fix->first->first = node->second;
					}

					if (parent_of_node_in_fix->first->first != nullptr) {
						//????
					}
					remove_from_node(parent_of_node_in_fix, parent_of_node_in_fix->kv[0]);
					delete son1;
					son1 = nullptr;
				}
				else if (son2 == node) {
					insert_to_node(son1, parent_of_node_in_fix->kv[0]);
					remove_from_node(parent_of_node_in_fix, parent_of_node_in_fix->kv[0]);
					if (node->first != nullptr) {
						son1->third = node->first;
					}
					else if (node->second != nullptr) {
						son1->third = node->second;
					}
					if (son1->third != nullptr) {
						//????
					}
					parent_of_node_in_fix->second = parent_of_node_in_fix->third;
					parent_of_node_in_fix->third = nullptr;
					delete son2;
					son2 = nullptr;
				}
				else if (son3 == node) {
					insert_to_node(son2, parent_of_node_in_fix->kv[1]);
					parent_of_node_in_fix->third = nullptr;
					remove_from_node(parent_of_node_in_fix, parent_of_node_in_fix->kv[1]);
					if (node->first != nullptr) {
						son2->third = node->first;
					}
					else if (node->second != nullptr) {
						son2->third = node->second;
					}

					if (son2->third != nullptr) {
						//????
					}

					delete son3;
					son3 = nullptr;
				}
			}
			else if (parent_of_node_in_fix->keysize == 2 and (son1->keysize == 2 or son2->keysize == 2 or son3->keysize == 2)) {
				if (son3 == node) {
					if (node->first != nullptr) {
						node->second = node->first;
						node->first = nullptr;
					}


					insert_to_node(node, parent_of_node_in_fix->kv[1]);
					if (son2->keysize == 2) {
						parent_of_node_in_fix->kv[1] = son2->kv[1];
						remove_from_node(son2, son2->kv[1]);
						node->first = son2->third;
						son2->third = nullptr;
						if (node->first != nullptr) {} /*leaf->first->parent = leaf;*/
					}
					else if (son1->keysize == 2) {
						parent_of_node_in_fix->kv[1] = son2->kv[0];
						node->first = son2->second;
						son2->second = son2->first;
						if (node->first != nullptr)/* leaf->first->parent = leaf*/;

						son2->kv[0] = parent_of_node_in_fix->kv[0];
						parent_of_node_in_fix->kv[0] = son1->kv[1];
						remove_from_node(son1, son1->kv[1]);
						son2->first = son1->third;
						if (son2->first != nullptr) {} /*second->first->parent = second;*/
						son1->third = nullptr;
					}
				}
				else if (son2 == node) {
					if (son3->keysize == 2) {
						if (node->first == nullptr) {
							node->first = node->second;
							node->second = nullptr;
						}
						insert_to_node(son2, parent_of_node_in_fix->kv[1]);
						parent_of_node_in_fix->kv[1] = son3->kv[0];
						remove_from_node(son3, son3->kv[0]);
						son2->second = son3->first;
						if (son2->second != nullptr) {} //second->second->parent = second;
						son3->first = son3->second;
						son3->second = son3->third;
						son3->third = nullptr;
					}
					else if (son1->keysize == 2) {
						if (node->second == nullptr) {
							node->second = node->first;
							node->first = nullptr;
						}
						insert_to_node(son2, parent_of_node_in_fix->kv[0]);
						parent_of_node_in_fix->kv[0] = son1->kv[1];
						remove_from_node(son1, son1->kv[1]);
						son2->first = son1->third;
						if (son2->first != nullptr) {}//second->first->parent = second;
						son1->third = nullptr;
					}
				}
				else if (son1 == node) {
					if (node->first == nullptr) {
						node->first = node->second;
						node->second = nullptr;
					}
					insert_to_node(son1, parent_of_node_in_fix->kv[0]);
					if (son2->keysize == 2) {
						parent_of_node_in_fix->kv[0] = son2->kv[0];
						remove_from_node(son2, son2->kv[0]);
						son1->second = son2->first;
						if (son1->second != nullptr) {} //first->second->parent = first;
						son2->first = son2->second;
						son2->second = son2->third;
						son2->third = nullptr;
					}
					else if (son3->keysize == 2) {
						parent_of_node_in_fix->kv[0] = son2->kv[0];
						son2->kv[0] = parent_of_node_in_fix->kv[1];
						parent_of_node_in_fix->kv[1] = son3->kv[0];
						remove_from_node(son3, son3->kv[0]);
						son1->second = son2->first;
						if (son1->second != nullptr) {} //first->second->parent = first;
						son2->first = son2->second;
						son2->second = son3->first;
						if (son2->second != nullptr) {} //second->second->parent = second;
						son3->first = son3->second;
						son3->second = son3->third;
						son3->third = nullptr;
					}
				}
			}
			else if (parent_of_node_in_fix ->keysize == 1) {
				insert_to_node(node, parent_of_node_in_fix->kv[0]);

				if (son1 == node and son2->keysize == 2) {
					parent_of_node_in_fix->kv[0] = son2->kv[0];
					remove_from_node(son2, son2->kv[0]);

					if (node->first == nullptr) node->first = node->second;

					node->second = son2->first;
					son2->first = son2->second;
					son2->second = son2->third;
					son2->third = nullptr;
					if (node->second != nullptr) {} //leaf->second->parent = leaf;
				}
				else if (son2 == node && son1->keysize == 2) {
					parent_of_node_in_fix->kv[0] = son1->kv[1];
					remove_from_node(son1, son1->kv[1]);
					if (node->second == nullptr) node->second = node->first;
					node->first = son1->third;
					son1->third = nullptr;
					if (node->first != nullptr) {} //leaf->first->parent = leaf;
				}
			}
			return parent_of_node_in_fix;
		}
	


		Node_23<T1, T2>* merge(Node_23<T1, T2>* node,Node_23<T1,T2>*root) {
			if (parent_of_node_in_fix->first == node) {
				insert_to_node(parent_of_node_in_fix->second,parent_of_node_in_fix->kv[0]);
				parent_of_node_in_fix->second->third = parent_of_node_in_fix->second->second;
				parent_of_node_in_fix->second->second = parent_of_node_in_fix->second->first;

				if (node->first != nullptr) parent_of_node_in_fix->second->first = node->first;
				else if (node->second != nullptr) parent_of_node_in_fix->second->first = node->second;

				if (parent_of_node_in_fix->second->first != nullptr) parent_of_node_in_fix->second->first->parent = parent_of_node_in_fix->second;

				remove_from_node(parent_of_node_in_fix,parent_of_node_in_fix->kv[0]);
				delete parent_of_node_in_fix->first;
				parent_of_node_in_fix->first = nullptr;
			}
			else if (parent_of_node_in_fix->second == node) {
				insert_to_node(parent_of_node_in_fix->first,parent_of_node_in_fix->kv[0]);

				if (node->first != nullptr) parent_of_node_in_fix->first->third = node->first;
				else if (node->second != nullptr) parent_of_node_in_fix->first->third = node->second;

				if (parent_of_node_in_fix->first->third != nullptr) parent_of_node_in_fix->first->third->parent = parent_of_node_in_fix->first;

				remove_from_node(parent_of_node_in_fix,parent_of_node_in_fix->kv[0]);
				delete parent_of_node_in_fix->second;
				parent_of_node_in_fix->second = nullptr;
			}

			Node_23<T1, T2>* tmp = nullptr;
			Node_23<T1, T2>* grandparent = parent();
			if (root == node) {
				grandparent = nullptr;
			}
			if (root->first == parent_of_node_in_fix or root->second == parent_of_node_in_fix or root->third == parent_of_node_in_fix) {
				grandparent = root;
			}
			if (grandparent == nullptr) {
				
				if (parent_of_node_in_fix->first != nullptr) {
					tmp = parent_of_node_in_fix->first;
				}
				else {
					tmp = parent_of_node_in_fix->second;
				}
				//tmp->parent = nullptr;
				parent_of_node_in_fix = nullptr;
				return tmp;
			}
			tmp = parent_of_node_in_fix;
			parent_of_node_in_fix = grandparent;
			return tmp;

		}



	};

	Insert* insrt_in_23;
	Remove* rmv_in_23;
	Search* srch_in_23;


	virtual void Remove_template(Node_23<T1, T2>*& root, T1 key) {
		
	}
	virtual void Insert_template(Node_23<T1, T2>*& root, T1 key, T2 value) {
		insrt_in_23->__insert(root, key, value);
	}

	virtual void __prefix(Node_23<T1, T2>* root, bool (*cb_ptr)(pair<T1,T2>* pairs, int depth)) {
		if (root == nullptr) {
			/*depth_for_traverse--;*/
			return;

		}
		else {
			cb_ptr(root->kv,depth_for_traverse);
			depth_for_traverse++;
			__prefix(root->first, *cb_ptr);
			__prefix(root->second, *cb_ptr);
			__prefix(root->third, *cb_ptr);
			depth_for_traverse--;
		}
	}
};
