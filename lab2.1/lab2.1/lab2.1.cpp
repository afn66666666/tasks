// lab2.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <string>
#include <iostream>
#include "Binary_Tree.h"
#include "AVL.h"
#include "Splay.h"
#include "Red_Black.h"
#include "exceptions_nodes_other.h"

bool output_func(int key, const char* value, int depth,int bf) {
    cout << "this is node with key : " << key << " and value : " << value << " depth : " << depth << " and balance : " << bf << endl;
    return true;
}

int main()
{

	try {
		Comparator<int>cmp;
		AVL<int, const char*>avl(cmp);
		for (int i = 0; i < 263; i++) {
			avl.insert(i, "sadewd");
		}
		avl.prefix(output_func);
		for (int i = 30; i > 0; i--) {
			if (i == 30) {
				for (int j = 220; j > 183; j--) {
					//avl.remove(j);
					//avl.prefix(output_func);
					///*avl.search(232);*/
					//cout << "" << "\ndeleted key is\n" << j << endl;
				}
			}
			avl.remove(i);
			if (i * 2 > 30) {
				avl.remove(i * 2);
				cout << "" << "deleted key is" << i * 2 << endl;
			}
			cout << "" << "deleted key is" << i << endl;
			
		}
		//cout << "\n\n\n";
		avl.prefix(output_func);
		//cout << "result of search is: " << (avl.search(232))->key << "\n\n\n\n\n\n\n\n";


		//BST<int, const char*>bst(cmp);
		//for (int i = 0; i < 263; i++) {
		//	bst.insert(i, "sadewd");
		//}
		//bst.prefix(output_func);
		//for (int i = 30; i > 0; i--) {
		//	bst.remove(i);
		//	if (i * 2 > 30) {
		//		bst.remove(i * 2);
		//		cout << "" << "deleted key is" << i * 2 << endl;
		//	}
		//	cout << "" << "deleted key is" << i << endl;

		//}
		//cout << "\n\n\n";
		//bst.prefix(output_func);
		//cout << "result of search is: " << (bst.search(232))->key;








	}
	catch (insert_exc& ex) {
		cout << ex.what();
	}
	catch (search_exc& ex) {
		cout << ex.what();
	}
	catch (remove_exc& ex) {
		cout << ex.what();
	}
}


/*BST<int, const char*> bst(cmp);
		bst.insert(13, "aasdas");
		bst.insert(0, "aasdas");
		bst.insert(-45, "aasdas");
		bst.insert(110, "aasdas");
		bst.insert(11, "aasdas");
		bst.insert(24, "aasdas");
		bst.insert(-1, "aasdas");
		bst.insert(133, "aasdas");
		bst.insert(3, "aasdas");
		bst.insert(-7, "aasdas");
		bst.insert(-110, "aasdas");
		bst.insert(-12, "aasdas");
		bst.insert(5, "aasdas");
		bst.insert(1000, "aasdas");
		bst.prefix(output_func);*/