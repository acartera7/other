#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>
#include <unordered_map>


using namespace std;

struct Node {
    string name = "";
    unordered_map<string,string> attributes;
    Node* parent = nullptr;
    vector<struct Node*> children;
};

void deleteNode(Node* n) {
    if(n) {
      if(!n->children.empty())
        for(auto* child : n->children) {
          deleteNode(child);
        }
      delete n;
    }
}

void deleteList(vector<Node*>& v) {
  for(Node* x:v) {
    deleteNode(x);
  }
  v.clear();
}

void answerQueries(vector<Node*>& v) {
  string line;
  string name;
  string key;

  while(getline(cin, line)) {
    
    if(line.find("~") != string::npos) { 
      
      //split dereference from nested objects calls
      name = line.substr(0,line.find("~"));
      line.erase(0,line.find("~")+1);
      key = line;

    } else {
      cerr << "Error: no dereference character \"~\" present" << endl;
    }

    string outer = name.substr(0,name.find("."));
    name.erase(0,name.find("."));
    Node* curr_node = nullptr;

    auto v_it = find_if(v.begin(),v.end(),[outer](Node* x){return outer == x->name;});
    if(v_it == v.end()) {
      cerr << "Not found!" << endl;
      continue;; 
    }  
      
    curr_node = *v_it;
    string next;

    while (name[0] == '.') { //find object requested withing object to be dereferenced
      name.erase(0,1); // remove "."
      if(name.find(".") != string::npos) {// check for final subquery
        next = name.substr(0,name.find("."));
        name.erase(0,name.find("."));
      } else
        next = name;
      
      auto& curr_node_children = curr_node->children;
      auto child_it = find_if(curr_node_children.begin(),curr_node_children.end(),[next](Node* x){return next == x->name;});
      
      if(child_it != curr_node_children.end()) {
        if ((*child_it)->name == next) {  
          curr_node = *child_it;
        }
      }
              
    } 
    // access the attribute value fro the object
    if (curr_node) {
      auto& attr = curr_node->attributes;
      if( attr.find(key) != attr.end()) {
        cout << attr[key] << endl;
      } else {
        cout << "Not Found!" << endl;
      }
    }
  }
}

vector<Node*> readHRML() {
    vector<Node*> result = {};
    int n,q;
    cin >> n >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string line;
    string token;
    Node* curr_node = nullptr;
    Node* prev      = nullptr;

     // input test
    //while(getline(cin, line)) {
    //  cout << line << endl;
    //}
    if(cin.peek() == EOF || char(cin.peek()) != '<') {
      return result;
    }
    
    while(char(cin.peek()) == '<') {
        getline(cin, line);
        
        if (line.find("</") != string::npos) {
          //close object
          line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
          string name =  line.substr(2,line.size()-3);
          
          if(name == curr_node->name) {
            if(!prev) {
              result.push_back(curr_node);
              curr_node = nullptr;
              continue;
            }
            curr_node = prev;
            prev = prev->parent;
          } else {
            cerr << "Error: error when closing object" << endl;
            exit(-1);
          }
          // check if outermost object, if so push into result vector
          
        } else {
        prev = curr_node;

        curr_node = new Node();
        
        if(prev) {
          curr_node->parent = prev;
          prev->children.push_back(curr_node);
        }

        //get name of tag
        curr_node->name = line.substr(1,line.find(" ")-1); 
        line = line.erase(0,line.find(" ")+1);
        
        //remove spaces
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        //remove last ">"
        line.erase(line.size()-1);
        
        string key, value;
        while (line.find('=') != string::npos){
          //find where next equals is
          key = line.substr(0, line.find('='));
          line.erase(0, line.find('=')+1);

          //get whatever is in the " "
          string temp = line.substr(0, line.find('\"',1)); // find second closing quote 
          value = temp.erase(0,1);
          curr_node->attributes.insert({key,value});
          line.erase(0,line.find('\"',1)+1);
        }

      } 
    }
    return result;
}

int main(void) {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   

    auto r = readHRML();
    answerQueries(r);
    deleteList(r);

    return 0;
}


