/*
 * Ledger.cc
 *
 *  Created on: Nov 13, 2025
 *      Author: root
 */
#include <omnetpp.h>
#include <string.h>
#include <iomanip>
#include "Ledger.h"

using namespace omnetpp;
Ledger::Ledger() {
}

void Ledger::addBlock(const Block& block) {
    //if(blocks.empty()){
        //throw std::runtime_error("No blocks in ledger");
    //}
    blocks.push_back(block);
    EV << "a new block generated!" << endl;
    EV << "Block_height:" << std::dec << block.blockheight << endl;
    //return blocks.back();
}

Block Ledger::getLatestBlock() const {
    if (blocks.empty()) {
        throw std::runtime_error("Ledger is empty. No latest block to retrieve.");
    }
    return blocks.back();
}

bool Ledger::verifyBlock(const Block& block) const{
    std::string oringinalHash=block.BlockHash;
    const_cast<Block&>(block).calculateHash();
    return(oringinalHash == block.BlockHash);
}

void Ledger::printLedger()const{
    for (const auto& block : blocks) {
            std::cout << "Block Height: " << block.blockheight << ", Hash: " << block.BlockHash << std::endl;
        }
}

Block Ledger::getBlock(int blockNum){
    if(blockNum < 0 || blockNum >= blocks.size()){
        throw std::out_of_range("Block number out of range");
    }
    return blocks[blockNum];
}


