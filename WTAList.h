#ifndef WTALIST_H
#define WTALIST_H
#include <iostream>
#include <fstream>
using namespace std;

struct WTAListNode
{
	int id;
	float distance;
	WTAListNode *next;
	WTAListNode *front;
	WTAListNode(int i, float d, WTAListNode *n = NULL, WTAListNode *f = NULL) {
		id = i;
		distance = d;
		next = n;
		front = f;
	}
};

class WTAList
{
public:
	WTAList(int temp_maxNum);
	~WTAList();
	bool empty() const;
	bool isFull() const;
	void clear();
	int size() const;
	bool push(int temp_id, float temp_distance);
	void print() const {
		WTAListNode *p = head;
		while (p) {
			printf("%d,%f -> ", p->id, p->distance);
			p = p->next;
		}
		cout << "NULL" << endl;
	}
	//show the list in file
	bool print_in_file(ofstream& out) const {
		if(out == nullptr) return false;
		WTAListNode *p = head;
		while (p) {
			out << p->id << " -> ";
			p = p->next;
		}
		out << "NULL\n" << endl;
	}
	int pop() {
		if (head) {
			int ret = head->id;
			WTAListNode *p = head;
			head = head->next;
			delete p;
			p = NULL;
			count--;
			return ret;
		}
		cout << "[WTAList] empty list!" << endl;
		return 0;
	}
	
private:
	int count;
	int maxNum; // maxNum: KNN中的K
	WTAListNode *head;
	WTAListNode *tail;
	bool push_back(int temp_id, float temp_distance);
	void popTailWhenCountGreaterThanMaxNum() {
		// if count > maxNum, pop the tail element
		if (count > maxNum) {
			WTAListNode *p = tail->front;
			delete tail;
			tail = p;
			tail->next = NULL;

			count--;
		}
	};
};

WTAList::WTAList(int temp_maxNum) {
	count = 0;
	maxNum = temp_maxNum;
	head = NULL;
	tail = NULL;
};

WTAList::~WTAList() {
	clear();
};

void WTAList::clear() {
	WTAListNode *current_node = head, *previous;
	while (current_node) {
		previous = current_node;
		current_node = current_node->next;
		delete previous;
	}
	count = 0;
	head = NULL;
	tail = NULL;
};

int WTAList::size() const{
	return count;
}

bool WTAList::empty() const{
	return count == 0;
};

bool WTAList::isFull() const{
	return count == maxNum;
}

bool WTAList::push_back(int temp_id, float temp_distance) {
	if (empty()) {
		head = new WTAListNode(temp_id, temp_distance);
		tail = head;
	}
	else {
		WTAListNode *newNode = new WTAListNode(temp_id, temp_distance);
		if (newNode == NULL) {
			cout << "[ERROR] From WTAList.h: WTAList Overflow!" << endl;
			return false;
		}
		newNode->front = tail;
		WTAListNode *p = tail;
		p->next = newNode;
		tail = newNode;	
	}
	count++;
	popTailWhenCountGreaterThanMaxNum();
	return true;
};

bool WTAList::push(int temp_id, float temp_distance) {
	// insert after the tail
	if (empty() || temp_distance <= tail->distance) {
		if (isFull())
			return true;
		return push_back(temp_id, temp_distance);
	}
	// insert before the head
	if (temp_distance >= head->distance) {
		WTAListNode *newNode = new WTAListNode(temp_id, temp_distance, head);
		if (newNode == NULL) {
			cout << "[ERROR] From WTAList.h: WTAList Overflow!" << endl;
			return false;
		}
		head->front = newNode;
		head = newNode;
		count++;
		popTailWhenCountGreaterThanMaxNum();
		return true;
	}
	// insert in the list
	WTAListNode *current_node, *following_node;
	current_node = head;
	following_node = current_node->next;
	while (1) {
		if (current_node->distance >= temp_distance && temp_distance > following_node->distance) {
			WTAListNode *newNode = new WTAListNode(temp_id, temp_distance, following_node, current_node);
			if (newNode == NULL) {
				cout << "[ERROR] From WTAList.h: WTAList Overflow!" << endl;
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
	cout << "[ERROR] From WTAList.h: push error!" << endl;
	return false;
};

#endif
