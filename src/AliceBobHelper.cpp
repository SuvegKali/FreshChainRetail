// This code is provided as requested by the prompt directly (Alice and Bob problem), 
// keeping it functionally isolated from the FreshChain Retail LLD architecture to preserve structure.
#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
using namespace std;

void helper(int n, vector<int> bars) {
    int i = 0, j = n-1, aliceTime = 0, bobTime = 0;
    int alice = 0, bob = 0;
    
    while(i <= j) {
        if(aliceTime > bobTime) {
            // bob will eat now
            bobTime += bars[j--];
            bob++;
        }
        else{
            // alice will eat now
            aliceTime += bars[i++];
            alice++;
        }
    }
    
    cout << alice << " " << bob << endl;
}

// NOTE: Not adding main() here to avoid conflict with the primary retail LLD main.cpp.

