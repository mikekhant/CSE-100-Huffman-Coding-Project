#include "HCTree.hpp"
#include "Helper.hpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    if(argc < 3) {
        error("Need 3 arguments");
    }
    //exception for wrong filename?

    FancyInputStream in(argv[1]);
    FancyOutputStream out(argv[2]);
    HCTree ht;

    int filesize = in.filesize();
    if(filesize == 0) {
        return 0;
    }
    vector<int> freqs (256,0);

    //read file header to build Huffman Tree
    int count = 0;
    int nextint = 0;
    for(int i=0; i<256; i++) {
        nextint = in.read_int();
        freqs[i] = nextint;
        count += nextint;
    }

    /*for(int i=0; i<256; i++) {
        for(int j=0; j<24; j++) {
            int temp = in.read_bit();
            nextint += temp * (2^j);
        }
        freqs[i] = nextint;
        count += nextint;
    }*/
    /*int qsize = in.read_int();
    int count = 1;
    for(int i=0; i<qsize; i++) {
        unsigned char nextbyte = in.read_byte();
        freqs[nextbyte] = count;
        //count++;
    }
    int total = in.read_int();*/

    ht.build(freqs);

    unsigned char nextchar;

    for(int i=0; i<count; i++) {
        nextchar = ht.decode(in);
        out.write_byte(nextchar);
    }
    
    freqs.clear();

}

