#include <cstddef>
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;	
class Node
{
  public:
    int data;
    Node *next;
    Node(int d){
      data=d;
      next=NULL;
    }
};
class Solution{
public:
  Node* removeDuplicates(Node *head) {
    if (head) {
      Node* curr_node = head;
      
      std::vector<int> seen;
      seen.push_back(curr_node->data);
      while(curr_node->next) {
        //if duplicate exists, skip it.
        if(std::find(seen.begin(), seen.end(), curr_node->next->data) != seen.end()) {
          curr_node->next = curr_node->next->next;
        } else {
          seen.push_back(curr_node->next->data);
          curr_node = curr_node->next;
        }
      }
    }
    return head;
  }

  Node* insert(Node *head,int data){
      Node* p=new Node(data);
      if(head==NULL){
        head=p;  

      }
      else if(head->next==NULL){
        head->next=p;

      }
      else{
        Node *start=head;
        while(start->next!=NULL){
          start=start->next;
        }
        start->next=p;   

      }
      return head;
    
  
  }
  void display(Node *head)
  {
      Node *start=head;
      while(start)
      {
        cout<<start->data<<" ";
        start=start->next;
      }
    }
};
			
int main()
{
	Node* head=NULL;
  	Solution mylist;
  int T,data;
  cin>>T;
  while(T-->0){
    cin>>data;
    head=mylist.insert(head,data);
  }	
  head=mylist.removeDuplicates(head);

	mylist.display(head);
		
}