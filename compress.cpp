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

    FancyInputStream instream(argv[1]);
    FancyOutputStream outstream(argv[2]);
    HCTree ht;

    int filesize = instream.filesize();
    if(filesize == 0) {
        return 0;
    }
    vector<int> freqs (256,0);

    int count = 0;
    // read bytes from original file and store frequencies in freqs
    for(int i=0; i<filesize; i++) {
        int nextbyte = instream.read_byte();
        count++;
        freqs[nextbyte] = freqs[nextbyte] + 1;
    }
    instream.reset();

    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq = ht.build(freqs);

    //file header for uncompress
    for(int i=0; i<256; i++) {
        outstream.write_int((int) freqs[i]);
    }
    /*for(int i=0; i<256; i++) {
        int binary[24] = {0};
        int frequency = freqs[i];
        int j = 0;
        while(frequency > 0) {
            binary[j] = frequency % 2;
            frequency = frequency / 2;
            j++;
        }
        for(int i=0; i<24; i++) {
            outstream.write_bit(binary[i]);
        }
    }*/
    
    /*int qsize = pq.size();
    outstream.write_int(qsize);
    for(int i=0; i<qsize; i++) {
        HCNode* temp = pq.top();
        pq.pop();
        unsigned char symbol = temp->symbol;
        outstream.write_byte(symbol);
    }
    outstream.write_int(count);*/

    //encode original file into output file
    for(int i=0; i<filesize; i++) {
        //int nextbit = instream.read_byte();
        //unsigned char symbol = (unsigned char) nextbit;
        ht.encode(instream.read_byte(), outstream);
    }

    freqs.clear();
    
    return 0;

}
