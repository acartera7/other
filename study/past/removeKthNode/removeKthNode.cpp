#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

class SinglyLinkedListNode {
    public:
        int data;
        SinglyLinkedListNode *next;

        SinglyLinkedListNode(int node_data) {
            this->data = node_data;
            this->next = nullptr;
        }
};

class SinglyLinkedList {
    public:
        SinglyLinkedListNode *head;
        SinglyLinkedListNode *tail;

        SinglyLinkedList() {
            this->head = nullptr;
            this->tail = nullptr;
        }

        void insert_node(int node_data) {
            SinglyLinkedListNode* node = new SinglyLinkedListNode(node_data);

            if (!this->head) {
                this->head = node;
            } else {
                this->tail->next = node;
            }

            this->tail = node;
        }
};

void print_singly_linked_list(SinglyLinkedListNode* node, string sep) {
    while (node) {
        cout << node->data;

        node = node->next;

        if (node) {
            cout << sep;
        }
    }
}

SinglyLinkedListNode* rec_removeKthNodeFromEnd(SinglyLinkedListNode* head, int& k) {
    if(head) {
      SinglyLinkedListNode* next = rec_removeKthNodeFromEnd(head->next,k);
      if(k == 0) {
          --k;
          delete head;
          return next;
      }
      head->next = next;
      --k;
      return head;

    }
    return nullptr;
}
 

SinglyLinkedListNode* removeKthNodeFromEnd(SinglyLinkedListNode* head, int k) {
    return rec_removeKthNodeFromEnd(head, k);
}


int main(void) {
  string line;
  stringstream ss;
  //cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin,line);
  int head_count = stoi(line);

  SinglyLinkedList* head = new SinglyLinkedList();

  for (int i = 0; i < head_count; i++) {
    string head_item_temp;
    getline(cin, head_item_temp);

    int head_item = stoi(head_item_temp);

    head->insert_node(head_item);
  }
  
  string k_temp;
  getline(cin, k_temp);

  int k = stoi(k_temp);

  SinglyLinkedListNode* result = removeKthNodeFromEnd(head->head, k);

  print_singly_linked_list(result, "\n");
  cout << "\n";

  return 0;

  ss.clear();


  return 0;
}