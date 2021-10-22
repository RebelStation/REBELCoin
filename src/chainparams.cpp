// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
// Copyright (c) 2021 The REBEL Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "utilstrencodings.h"

#include <assert.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(std::make_shared<const CTransaction>(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

void CChainParams::UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    assert(IsRegTestNet()); // only available for regtest
    assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
    consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Electric powered plane trialled over Orkney";
    const CScript genesisOutputScript = CScript() << ParseHex("04c10b83b2703ddf322a7dbd62dd5815aa7c10bd055815cf121ba72607c573b8810c02c05b2aed05b4abb9c4b77b26d92428c61256cd42774baaea0a073b2bd0c4") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints = {
    {0, uint256S("0x00000d62e04e27c83d0d2b9d37df1f8cee6bdef1c429125140914f6e9ed20018")},
    {1, uint256S("0x00000d2aeeb16e119ee38b1e62d4d282fe7f61195ea113f210d65411aa6a6304")},
    {1402, uint256S("0xfb5b0055e2db94b4ae7260003c7fecef87b64e7c8300e17ff92f3f0f2f16725d")},
    {3789, uint256S("0x3d775489759a33128094587caa00aa77b471b87f78de64de651799967baf3126")},
    {10174, uint256S("0xbf21d1ec00e42978b147ecd1c748f8605196b677696391e07272220d563467c0")},
    {19871, uint256S("0xa22cb24c2096b503716e9057c2d2efe80470155f2defb18b241fb5b4ccfd5685")},
    {26606, uint256S("0xdb46923472f36b802b155bc8560480640b35c08ec91defaceffe7803d4d4b423")},
    {31563, uint256S("0x52ecaf67c80ccfedfe3419ce49bf04f511e9bedd064a211c0269618615508606")},
    {36917, uint256S("0x3fb36031decdc9624122e5fc9547821080ffee527c1ffa96bf5a4880d4b5b5c5")},

};

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1634634660, // * UNIX timestamp of last checkpoint block
    73840,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the UpdateTip debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet = {
    {0, uint256S("0x")},
};

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1628736510,
    18912,
    2000};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest = {{0, uint256S("0x001")}};
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        strNetworkID = "main";

        genesis = CreateGenesisBlock(1629950400, 1712266, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000d62e04e27c83d0d2b9d37df1f8cee6bdef1c429125140914f6e9ed20018"));
        assert(genesis.hashMerkleRoot == uint256S("0x00bded9a306ded5ff30a2ec0ac7ff899ac84ac9daaf8168169210f0fd394f5f7"));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // REBELCoin starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 21600;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 60;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 21000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 60;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 2 * 60;
        consensus.nTimeSlotLength = 15;
		consensus.strTreasuryAddr = "Rspwju8UoHW2DKjzujeNiv8nraHTWXGhRB";
		consensus.nTreasuryStartHeight = 200;
        consensus.nMaxProposalPayments = 6;

        // spork keys
        consensus.strSporkPubKey = "044d17d333e53364a68a718e8b3e11468622a73197d3a808a4f492bf9410c5692766fc504f9b27a233d5546327b6f18b58820d39b40333a64b6aaf79fd5b4dd1a9";
        consensus.strSporkPubKeyOld = "044d17d333e53364a68a718e8b3e11468622a73197d3a808a4f492bf9410c5692766fc504f9b27a233d5546327b6f18b58820d39b40333a64b6aaf79fd5b4dd1a9";
        consensus.nTime_EnforceNewSporkKey = 1630040400; //!> Friday, 27 August 2021 5:00:00 AM GMT
        consensus.nTime_RejectOldSporkKey = 1630134000;  //!> Saturday, 28 August 2021 7:00:00 AM GMT


        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 1;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 400;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 250;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 300;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          = 300;

        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock =
                uint256S("0x");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock =
                uint256S("0x");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock =
                uint256S("0x");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x3b;
        pchMessageStart[1] = 0xcd;
        pchMessageStart[2] = 0x6a;
        pchMessageStart[3] = 0x2b;
        nDefaultPort = 18150;

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("159.203.22.209", "159.203.22.209", true); 
        vSeeds.emplace_back("2604:a880:cad:d0::bcf:d001", "2604:a880:cad:d0::bcf:d001", true); 
        vSeeds.emplace_back("206.189.140.184", "206.189.140.184", true); 
        vSeeds.emplace_back("2400:6180:100:d0::8e9:3001", "2400:6180:100:d0::8e9:3001", true); 
        vSeeds.emplace_back("192.81.212.42", "192.81.212.42", true); 
        vSeeds.emplace_back("2604:a880:400:d0::1631:7001", "2604:a880:400:d0::1631:7001", true); 
        vSeeds.emplace_back("147.182.238.115", "147.182.238.115", true); 
        vSeeds.emplace_back("2604:a880:4:1d0::378:c000", "2604:a880:4:1d0::378:c000", true); 
        vSeeds.emplace_back("68.183.1.212", "68.183.1.212", true); 
        vSeeds.emplace_back("2a03:b0c0:2:d0::ec6:c001", "2a03:b0c0:2:d0::ec6:c001", true); 
        vSeeds.emplace_back("157.230.241.189", "157.230.241.189", true); 
        vSeeds.emplace_back("2400:6180:0:d0::10e9:8001", "2400:6180:0:d0::10e9:8001", true); 
        vSeeds.emplace_back("178.62.108.100", "178.62.108.100", true); 
        vSeeds.emplace_back("2a03:b0c0:1:d0::2c:1", "2a03:b0c0:1:d0::2c:1", true); 
        vSeeds.emplace_back("167.99.248.255", "167.99.248.255", true); 
        vSeeds.emplace_back("2a03:b0c0:3:d0::d0d:7001", "2a03:b0c0:3:d0::d0d:7001", true); 

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 61);		// starting with 'R'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 48);     // starting with 'L'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x02, 0x2D, 0x25, 0x33};
        base58Prefixes[EXT_SECRET_KEY] = {0x02, 0x21, 0x31, 0x2B};
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x77};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "rs";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "rviews";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "rblks";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "r-secret-spending-key-main";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "rxviews";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};

/**
 * Testnet (v5)
 */
class CTestNetParams : public CChainParams
{
public:
    CTestNetParams()
    {
        strNetworkID = "test";

        genesis = CreateGenesisBlock(1627545600, 706439, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
     //   assert(consensus.hashGenesisBlock == uint256S("0x0000043b7645dd2400798220c23d2ff104759202d605f53ca667ad1641d354d5"));
     //   assert(genesis.hashMerkleRoot == uint256S("0xc0f9c69478ec82521d94782a5daff8e95acbcf3c9188424db61f5b6dc16fa7f7"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // REBELCoin starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 60;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 20000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 60;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 2 * 60;
        consensus.nTimeSlotLength = 15;
		consensus.strTreasuryAddr = "";
		consensus.nTreasuryStartHeight = 800;
        consensus.nMaxProposalPayments = 6;


        // spork keys
        consensus.strSporkPubKey = "042808e26a20badc81302345a42d82dd9079e0bc08be509069f35c006aca29b69210be598356b7dd6b2cada8c27827085295658e745819278c40760e9b585c6195";
        consensus.strSporkPubKeyOld = "042808e26a20badc81302345a42d82dd9079e0bc08be509069f35c006aca29b69210be598356b7dd6b2cada8c27827085295658e745819278c40760e9b585c6195";
        consensus.nTime_EnforceNewSporkKey = 1627624800;    //!> Friday, 30 July 2021 6:00:00 AM GMT
        consensus.nTime_RejectOldSporkKey = 1627718400;     //!> Saturday, 31 July 2021 8:00:00 AM GMT


        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 1;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 400;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 250;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 300;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          = 400;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xa4;
        pchMessageStart[1] = 0xe1;
        pchMessageStart[2] = 0xc3;
        pchMessageStart[3] = 0x5a;
        nDefaultPort = 11012;

        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("95.179.151.253", "95.179.151.253", true);   
        vSeeds.emplace_back("2001:19f0:5001:761:5400:03ff:fe7c:eaa2", "2001:19f0:5001:761:5400:03ff:fe7c:eaa2", true);     
		vSeeds.emplace_back("136.244.105.209", "136.244.105.209", true);
		vSeeds.emplace_back("2001:19f0:5001:23e5:5400:03ff:fe76:51ac", "2001:19f0:5001:23e5:5400:03ff:fe76:51ac", true);
		vSeeds.emplace_back("78.141.222.206", "78.141.222.206", true);
		vSeeds.emplace_back("2001:19f0:5001:154a:5400:03ff:fe72:e27a", "2001:19f0:5001:154a:5400:03ff:fe72:e27a", true);
		vSeeds.emplace_back("95.179.144.51", "95.179.144.51", true);
		vSeeds.emplace_back("2001:19f0:5001:fa6:5400:03ff:fe72:e279", "2001:19f0:5001:fa6:5400:03ff:fe72:e279", true);
		vSeeds.emplace_back("108.61.117.150", "108.61.117.150", true);
		vSeeds.emplace_back("2001:19f0:5001:234c:5400:03ff:fe72:e278", "2001:19f0:5001:234c:5400:03ff:fe72:e278", true);
        vSeeds.emplace_back("45.32.233.230", "45.32.233.230", true);
		vSeeds.emplace_back("2001:19f0:5001:1bb:5400:03ff:fe72:e277", "2001:19f0:5001:1bb:5400:03ff:fe72:e277", true);
        vSeeds.emplace_back("78.141.218.194", "78.141.218.194", true);
		vSeeds.emplace_back("2001:19f0:5001:1a9e:5400:03ff:fe72:e276", "2001:19f0:5001:1a9e:5400:03ff:fe72:e276", true);
        vSeeds.emplace_back("95.179.153.109", "95.179.153.109", true);
		vSeeds.emplace_back("2001:19f0:5001:35f6:5400:03ff:fe72:e275", "2001:19f0:5001:35f6:5400:03ff:fe72:e275", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 92); // Testnet RBL addresses start with 'e'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 125);     // starting with 's'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c'
        // Testnet rebelcoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = {0x3a, 0x80, 0x61, 0xa0};
        // Testnet rebelcoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = {0x3a, 0x80, 0x58, 0x37};
        // Testnet rebelcoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x01};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "rblktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-test";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviewtestsapling";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams
{
public:
    CRegTestParams()
    {
        strNetworkID = "regtest";

        genesis = CreateGenesisBlock(1454124731, 2402015, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
   //     assert(consensus.hashGenesisBlock == uint256S("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));
    //    assert(genesis.hashMerkleRoot == uint256S("0x1b2ef6e2f28be914103a277377ae7729dcd125dfeb8bf97bd5964ba72b6dc39b"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // REBELCoin starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 2;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
		consensus.strTreasuryAddr = "";
		consensus.nTreasuryStartHeight = 100;
        consensus.nMaxProposalPayments = 20;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 251;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 251;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 251;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 300;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          = 300;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 51476;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet rebelcoin addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet rebelcoin script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet rebelcoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = {0x3a, 0x80, 0x61, 0xa0};
        // Testnet rebelcoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = {0x3a, 0x80, 0x58, 0x37};
        // Testnet rebelcoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x01};

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "rblktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-test";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviewtestsapling";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params()
{
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    globalChainParams->UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
