#pragma once


using namespace std;

class tree_exception :public exception {
public:
	tree_exception(const char* message) :exception(message) {}
};

class insert_exc : public tree_exception {
public:
	insert_exc(const char* message) :tree_exception(message) {}
};

class remove_exc : public tree_exception {
public:
	remove_exc(const char* message) :tree_exception(message) {}
};

class search_exc : public tree_exception {
public:
	search_exc(const char* message) : tree_exception(message) {}
};



union bf {
	int avl = 0;
	bool rb;
};
typedef union bf balance_factor;


template <typename T1>
class Comparator {
public:
	virtual int compare(T1 k1, T1 k2) {
		if (k1 < k2) {
			return 1;
		}
		else if (k1 == k2) {
			return 0;
		}
		else {
			return -1;
		}
		return 0;
	}

};


template <typename T1, typename T2>
struct AC_Node {
	T1 key;
	T2 value;
	AC_Node* left;
	AC_Node* right;
	balance_factor bf;

};