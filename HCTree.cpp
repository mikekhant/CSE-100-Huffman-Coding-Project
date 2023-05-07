#include "HCTree.hpp"
#include "Helper.hpp"
#include <queue>
#include <vector>
#include <iostream>
using namespace std;

void deletetree(HCNode* n) {
    if(n != nullptr) {
        if(n->c0 != nullptr) {
            deletetree(n->c0);
        }
        if(n->c1 != nullptr) {
            deletetree(n->c1);
        }
        delete(n);
    }
}

HCTree::~HCTree() {
    deletetree(root);
    root = nullptr;
}

priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> HCTree::build(const vector<int>& freqs) {
    for(int i=0; i<256; i++) {
        if(freqs[i] != 0) {
            leaves[i] = new HCNode(freqs[i], i);
        }
        else leaves[i] = nullptr;
    }
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for(int i=0; i<256; i++) {
        if(freqs[i] != 0) {
            pq.push(leaves[i]);
        }
    }
    
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> copy(pq);

    
    while(pq.size() > 1) {
        //Remove two smallest trees from forest
        HCNode* left = pq.top();
        pq.pop(); 
        HCNode* right = pq.top();
        pq.pop();
        

        // New node that will be root of new tree
        HCNode* treeroot = new HCNode(left->count + right->count, '\0');
        //cout << treeroot->count << endl;

        //Label edges as 0 and 1
        treeroot->c0 = left;
        treeroot->c1 = right;

        //Label new root as parent
        left->p = treeroot; right->p = treeroot;

        pq.push(treeroot);

    }
    root = pq.top();
    pq.pop();

    return copy;
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    int index = symbol;
    //temp is the HCNode of symbol
    HCNode* temp = leaves[index];

    string binary = "";
    //while temp HCNode is not at the root, 
    while(temp != root) {
        HCNode* tempparent = temp->p;

        //find out if the next edge is 0 or 1
        if(tempparent->c0 == temp) {
            binary = "0" + binary; 
        }
        else {
            binary = "1" + binary;
        }
        temp = temp->p;
    }
    // write bits to out stream
    for(int i=0; i<binary.length(); i++) {
        int wbit = stoi(binary.substr(i,1));
        out.write_bit(wbit);
    }
}

unsigned char HCTree::decode(FancyInputStream & in) const {
    //start at root
    HCNode* next = root;

    //while the temporary HCNode is not a leaf
    while(next->c0 != nullptr && next->c1 != nullptr) {
        //read next bit and traverse edges depending on if bit==0 or ==1
        int rbit = in.read_bit();
        if(rbit == 0) {
            next = next->c0;
        }
        else next = next->c1;
    }

    //return the symbol of the leaf HCNode
    return next->symbol;
}
