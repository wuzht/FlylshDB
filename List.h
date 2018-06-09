#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <fstream>
using namespace std;

struct ListNode
{
	int id;
	float distance;
	ListNode *next;
	ListNode *front;
	ListNode(int i, float d, ListNode *n = NULL, ListNode *f = NULL) {
		id = i;
		distance = d;
		next = n;
		front = f;
	}
};

class List
{
public:
	List(int temp_maxNum);
	~List();
	bool empty() const;
	bool isFull() const;
	void clear();
	int size() const;
	bool push(int temp_id, float temp_distance);
	void print() const {
		ListNode *p = head;
		while (p) {
			printf("%d -> ", p->id);
			p = p->next;
		}
		cout << "NULL" << endl;
	}
	//show the list in file
	bool print_in_file(ofstream& out) const {
		if(out == nullptr) return false;
		ListNode *p = head;
		while (p) {
			out << p->id << " -> ";
			p = p->next;
		}
		out << "NULL\n" << endl;
	}

	int get(int index);
	bool in(int number);
	
private:
	int count;
	int maxNum; // maxNum: KNN中的K
	ListNode *head;
	ListNode *tail;
	bool push_back(int temp_id, float temp_distance);
	void popTailWhenCountGreaterThanMaxNum() {
		// if count > maxNum, pop the tail element
		if (count > maxNum) {
			ListNode *p = tail->front;
			delete tail;
			tail = p;
			tail->next = NULL;

			count--;
		}
	};
};

List::List(int temp_maxNum) {
	count = 0;
	maxNum = temp_maxNum;
	head = NULL;
	tail = NULL;
};

List::~List() {
	clear();
};

void List::clear() {
	ListNode *current_node = head, *previous;
	while (current_node) {
		previous = current_node;
		current_node = current_node->next;
		delete previous;
	}
	count = 0;
	head = NULL;
	tail = NULL;
};

int List::size() const{
	return count;
}

bool List::empty() const{
	return count == 0;
};

bool List::isFull() const{
	return count == maxNum;
}

bool List::push_back(int temp_id, float temp_distance) {
	if (empty()) {
		head = new ListNode(temp_id, temp_distance);
		tail = head;
	}
	else {
		ListNode *newNode = new ListNode(temp_id, temp_distance);
		if (newNode == NULL) {
			cout << "[ERROR] From List.h: List Overflow!" << endl;
			return false;
		}
		newNode->front = tail;
		ListNode *p = tail;
		p->next = newNode;
		tail = newNode;	
	}
	count++;
	popTailWhenCountGreaterThanMaxNum();
	return true;
};

bool List::push(int temp_id, float temp_distance) {
	// insert after the tail
	if (empty() || temp_distance >= tail->distance) {
		if (isFull())
			return true;
		return push_back(temp_id, temp_distance);
	}
	// insert before the head
	if (temp_distance <= head->distance) {
		ListNode *newNode = new ListNode(temp_id, temp_distance, head);
		if (newNode == NULL) {
			cout << "[ERROR] From List.h: List Overflow!" << endl;
			return false;
		}
		head->front = newNode;
		head = newNode;
		count++;
		popTailWhenCountGreaterThanMaxNum();
		return true;
	}
	// insert in the list
	ListNode *current_node, *following_node;
	current_node = head;
	following_node = current_node->next;
	while (1) {
		if (current_node->distance <= temp_distance && temp_distance < following_node->distance) {
			ListNode *newNode = new ListNode(temp_id, temp_distance, following_node, current_node);
			if (newNode == NULL) {
				cout << "[ERROR] From List.h: List Overflow!" << endl;
				return false;
			}
			current_node->next = newNode;
			following_node->front = newNode;
			count++;
			popTailWhenCountGreaterThanMaxNum();
			return true;
		}
		current_node = current_node->next;
		following_node = current_node->next;
	}

	// error
	cout << "[ERROR] From List.h: push error!" << endl;
	return false;
};

int List::get(int index) {
	if(empty() || index >= size() || index < 0) return -1;
	ListNode* temp = head;
	for(int i = 1; i <= index; i++) {
		temp = temp->next;
	}
	return temp->id;
}

bool List::in(int number) {
	if(empty()) return false;
	ListNode* temp = head;
	while(temp) {
		if(temp->id == number) return true;
		temp = temp->next;
	}
	return false;
}

#endif
