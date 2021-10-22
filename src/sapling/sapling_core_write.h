// Copyright (c) 2015-2020 The Zcash developers
// Copyright (c) 2020 The PIVX developers
// Copyright (c) 2021 The REBEL Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#ifndef REBELCoin_SAPLING_CORE_WRITE_H
#define REBELCoin_SAPLING_CORE_WRITE_H

#include "primitives/transaction.h"
#include <univalue.h>

// Format Sapling tx information in json.
void TxSaplingToJSON(const CTransaction& tx, UniValue& entry);

#endif //REBELCoin_SAPLING_CORE_WRITE_H
