/*
 * Ledger.h
 *
 *  Created on: Nov 13, 2025
 *      Author: root
 */

#ifndef LEDGER_H_
#define LEDGER_H_

#include <vector>
#include <Block.h>

class Ledger{
public:
    std::vector<Block> blocks;
    Ledger();
    void addBlock(const Block& block);
    Block getLatestBlock() const;
    bool verifyBlock(const Block& block) const;
    void printLedger() const;
    Block getBlock(int blockNum);
};



#endif /* LEDGER_H_ */
