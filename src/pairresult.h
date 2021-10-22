// Copyright (c) 2019 The PIVX developers
// Copyright (c) 2021 The REBEL Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REBELCoin_PAIRRESULT_H
#define REBELCoin_PAIRRESULT_H


class PairResult {

public:
    PairResult(bool res):result(res){}
    PairResult(bool res, std::string* statusStr):result(res), status(statusStr){}

    bool result;
    std::string* status = nullptr;
};


#endif //REBELCoin_PAIRRESULT_H
