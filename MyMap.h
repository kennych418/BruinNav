// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#ifndef MYMAP_H
#define MYMAP_H

#include <string>
#include "support.h"
#include <queue>

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node {
		KeyType key;
		ValueType value;
		Node* left;
		Node* right;
	};
	Node* m_root;
	int m_size;
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap() {
	m_root = nullptr;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap() {
	clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear() {
	/*Node* k = m_root;
	Node* i = k;
	if (k == nullptr)
		return;
	while (m_size > 0) {
		i = k;
		while (i->left != nullptr || i->right != nullptr) {
			if (i->left != nullptr)
				i = i->left;
			else if (i->right != nullptr)
				i = i->right;
		}
		delete i;
		m_size--;
	}*/

	if (m_root == nullptr)
		return;

	queue<Node*> Nodequeue;
	Nodequeue.push(m_root);
	while (Nodequeue.empty() != true) {
		Node* it = Nodequeue.front();
		Nodequeue.pop();
		if (it->left != nullptr)
			Nodequeue.push(it->left);
		if (it->right != nullptr)
			Nodequeue.push(it->right);
		delete it;
	}
	m_root = nullptr;
}
template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const {
	return m_size;
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
	ValueType* tester = find(key);
	if (tester == nullptr) {
		Node* it = m_root;

		for (;;) {
			if (it == nullptr)
				break;
			if (it->key > key) {
				if (it->left != nullptr)
					it = it->left;
				else
					break;
			}
			if (it->key < key) {
				if (it->right != nullptr)
					it = it->right;
				else
					break;
			}
		}
		Node* temporary = new Node;
		temporary->left = nullptr;
		temporary->right = nullptr;
		temporary->value = value;
		temporary->key = key;

		if (it == nullptr)
			m_root = temporary;
		else {
			if (it->key > key)
				it->left = temporary;
			else
				it->right = temporary;
		}
		m_size++;
	}
	else {
		*tester = value;
	}
}
template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const {
	if (m_root == nullptr)
		return nullptr;

	Node* searchpointer = m_root;
	for (;;) {
		if (searchpointer->key > key) {
			if (searchpointer->left != nullptr)
				searchpointer = searchpointer->left;
			else
				break;
		}
		else if (searchpointer->key < key) {
			if (searchpointer->right != nullptr)
				searchpointer = searchpointer->right;
			else
				break;
		}
		else if (searchpointer->key == key)
			break;
	}

	if (searchpointer->key == key) {
		ValueType* result = &searchpointer->value;
		return result;
	}
	else
		return nullptr;
}

#endif 

