/*
 * Block.h
 *
 *  Created on: Nov 13, 2025
 *      Author: root
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <vector>
#include <string>
#include "Report.h"

class Block{
public:
    int blockheight;
    std::string previousBlockHash;
    long timestamp;
    std::vector<Report> reports;
    std::string BlockHash;
    Block(int height, const std::string & preHash, const std::vector<Report>& repos);
    void calculateHash();
    static Block createGenesisBlock();
};



#endif /* BLOCK_H_ */
