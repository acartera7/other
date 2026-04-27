#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

int sockMerchant(int n, vector<int> ar) {
    int pairs = 0;
    if (!ar.empty()) {
        vector<int> colors;
        for(int sock : ar){
            if(!colors.empty() ) {
                auto it = find(colors.begin(), colors.end(), sock);
                if(it != colors.end()){
                    colors.erase(it);   
                    ++pairs; 
                    continue;
                }   
            }
            colors.push_back(sock);
        }
    }
    return pairs;
}

int main(void) {
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "No Input" << endl;
        return 0; 
    }
    int n;
    inputFile >> n;
    vector<int> ar;
    for(int i=0; i<n; ++i) {
        int sock;
        inputFile >> sock;
        ar.push_back(sock);
    }
    inputFile.close();
    cout << sockMerchant(n, ar) << endl;
    return 0;
}

