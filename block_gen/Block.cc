/*
 * Block.cc
 *
 *  Created on: Nov 14, 2025
 *      Author: root
 */
#include <vector>
#include <string>
//#include <sstream>
#include <functional>
#include "Block.h"
#include <ctime>
#include "Report.h"

Block::Block(int height, const std::string &preHash, const std::vector<Report>& repos)
    : blockheight(height), previousBlockHash(preHash), timestamp(time(nullptr)), reports(repos) {
    calculateHash();
}

void Block::calculateHash() {
    //std::stringstream ss;
    std::string data = std::to_string(blockheight) + previousBlockHash + std::to_string(timestamp);
    for (const auto& report : reports) {
        data.append(report.toString());
        }
    std::size_t stringHash = std::hash<std::string>{}(data);
    BlockHash = std::to_string(stringHash);
}

Block Block::createGenesisBlock() {
    std::vector<Report> reports;
    //std::string genmes = "I am a genesisBlock";
    //std::size_t stringHash = std::hash<std::string>{}(genmes);
    //std::string genHash = std::to_string(stringHash);
    Report genesisReport;
    genesisReport.senderID = "system";
    genesisReport.content = "Genesis Block: The foundation of the blockchain.";
    genesisReport.timestamp = time(nullptr);
    reports.push_back(genesisReport);
    return Block(0, "0", reports);
}





