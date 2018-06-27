// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2018 The Dash Core && Momo Core developers developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "arith_uint256.h"
#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
void MineGenesisBlock(CBlock &genesis);
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
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Momocash start 21.02.2018 Donald Trump";
    const CScript genesisOutputScript = CScript() << ParseHex("04b05ad4a442424de84e89c9b9cbe504d0f4efdb5d82c51f546ed6ec2fd0664a3e5f2278ae611581c1de417926dda35d2190a96742c4dcab4dfb5f0c5da7dd7159") << OP_CHECKSIG;
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


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210240; // Note: actual number of blocks per calendar year with DGW v3 is ~200700 (for example 449750 - 249050)
        consensus.nMasternodePaymentsStartBlock = 50; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 158000; // actual historical value
        consensus.nMasternodePaymentsIncreasePeriod = 576*30; // 17280 - actual historical value
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 328008; // actual historical value
        consensus.nBudgetPaymentsCycleBlocks = 16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 614820; // The block at which 12.1 goes live (end of final 12.0 budget cycle)
        consensus.nSuperblockCycle = 16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x000007d91d1254d60e2dd1ae580383070a4ddffa4c64c2eeb4a2f9ecc0414343");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Momo: 1 day
        consensus.nPowTargetSpacing = 2.5 * 60; // Momo: 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 1;
        consensus.nPowDGWHeight = 2;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1486252800; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517788800; // Feb 5th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1520438400; // Oct 15th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1551974400; // Oct 15th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); 

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); 

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        // pchMessageStart[0] = 0xa2;
        // pchMessageStart[1] = 0x31;
        // pchMessageStart[2] = 0x1c;
        // pchMessageStart[3] = 0x32;
        pchMessageStart[0] = 0xa5;
        pchMessageStart[1] = 0xa1;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0x2d;
        vAlertPubKey = ParseHex("04d281fcbca43ccedf5f87f155a5d074d0394f7bea79632a9076a0dffce24bfe3527410e4fdf33d1c47d93339e6680f3019e53c724ba82bee1e7eb4c6be8337170");
        nDefaultPort = 7777;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1519142400,  2097483, 0x1e0ffff0, 1, 50 * COIN);

        // If genesis block hash does not match, then generate new genesis hash.

        // if (true && genesis.GetHash() != consensus.hashGenesisBlock)
        // {

        //             printf("genesis.nTime = %u \n", genesis.nTime);
        //             printf("genesis.nNonce = %u \n", genesis.nNonce);
        //             printf("genesis.nVersion = %u \n", genesis.nVersion);
        //             printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str()); //first this, then comment this line out and uncomment the one under.
        //             printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str()); //improvised. worked for me, to find merkle root
        // }
        consensus.hashGenesisBlock = genesis.GetHash();//no 0 block?
        // MineGenesisBlock(genesis);
        assert(consensus.hashGenesisBlock == uint256S("0x0000054897f9f6fc81d8da5cfe24cdb58664c81037cec0ecf7b18db06b9fc529"));
        assert(genesis.hashMerkleRoot == uint256S("0x33c100a156d136f2b76297bbf9d6df385bcc9379b4777307271c27f8f4a4efbb"));

        // vSeeds.push_back(CDNSSeedData("seed18.hellolua.com", "seed18.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed17.hellolua.com", "seed17.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed16.hellolua.com", "seed16.hellolua.com"));
		vSeeds.push_back(CDNSSeedData("seed15.hellolua.com", "seed15.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed14.hellolua.com", "seed14.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed13.hellolua.com", "seed13.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed12.hellolua.com", "seed12.hellolua.com"));
		// vSeeds.push_back(CDNSSeedData("seed11.hellolua.com", "seed11.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed10.hellolua.com", "seed10.hellolua.com"));
		vSeeds.push_back(CDNSSeedData("seed29.hellolua.com", "seed9.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed8.hellolua.com", "seed8.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed7.hellolua.com", "seed7.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed6.hellolua.com", "seed6.hellolua.com"));
		vSeeds.push_back(CDNSSeedData("seed5.hellolua.com", "seed5.hellolua.com"));
        // vSeeds.push_back(CDNSSeedData("seed4.hellolua.com", "seed4.hellolua.com"));
        vSeeds.push_back(CDNSSeedData("seed3.hellolua.com", "seed3.hellolua.com"));
        vSeeds.push_back(CDNSSeedData("seed21.hellolua.com", "seed2.hellolua.com"));
		vSeeds.push_back(CDNSSeedData("seed1.hellolua.com", "seed1.hellolua.com"));
        vSeeds.push_back(CDNSSeedData("seed0.hellolua.com", "seed0.hellolua.com"));
        vSeeds.push_back(CDNSSeedData("explorer.momocash.org", "explorer.momocash.org"));
        vSeeds.push_back(CDNSSeedData("pool.momocash.org", "pool.momocash.org"));
        vSeeds.push_back(CDNSSeedData("www.oxpools.com", "www.oxpools.com"));
        vSeeds.push_back(CDNSSeedData("pool.momopay.org", "pool.momopay.org"));
        vSeeds.push_back(CDNSSeedData("www.1024pool.com", "www.1024pool.com"));

        // Momo addresses start with 'M'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        // Momo script addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,16);
        // Momo private keys start with '7' or 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,204);
        // Momo BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Momo BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Momo BIP44 coin type is '5'
        nExtCoinType = 5;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04133d750437efa2d79794a1dafaaaa2c9f03248c49127c33201bb7fb6e06c24ebca21c8679f6d8315f6ffb7ef34405946db3940e43d91ce335df106e9562738ae";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (    0, uint256S("0x0000054897f9f6fc81d8da5cfe24cdb58664c81037cec0ecf7b18db06b9fc529"))
            (  500, uint256S("0x000000006b1adb488204d14fe9a3ef219f27ed6708252adffd15d98336f73241"))
            (  5000, uint256S("0x00000003e54716a6f42f8dc3cccfa2181e8819d7818d9ca10a3b734edbf5392f"))
            (  10000, uint256S("0x00000004abfa35a291638e3d1f2831d266a8e8b911b55c40768d340733fb999a"))
            (  13000, uint256S("0x00000001ad1be0596ccaed464e3e6b6e91f2a2468ad8d5d32128e017b0b5a1be"))
            (  13500, uint256S("0x0000000087a753540559a133ca2e4b2dae3480dacafcbba16cfdb3c36e14be31"))
            (  19800, uint256S("0x000000001205c192148dc5bcf9ab83a5f5cfff62e13f4254c395ff6f00027765"))
            (  25000, uint256S("0x0000000013bff6b6e1359e0ca3235b22bce0e66d702f30ca01e023b2521be0a7"))
            (  35000, uint256S("0x00000000011281937cac82c6c9c568a0e88da35167a126399fa9ebeb3f0e8017"))
            (  40466, uint256S("0x00000000254b8247893db31c189d7c5edae8b2b302f6bf24f9ab92684ac04fd8"))
            (  44283, uint256S("0x00000000105e3d919f03c233eb4f34900b10b2b72b64fd6478e10ba189bf0cf1"))
            (  44590, uint256S("0x0000000000ce6ecb3f493d011f69cb209acdc0e71832f0c5ff8551fae2989995")),
            // (  3000, uint256S("0x000000026d7b2f3e438c61ab84edc7415cc69bb5880bfd91738f8435978bc048")),
            1524812462, // * UNIX timestamp of last checkpoint block
            13921,       // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            5000         // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210240;
        consensus.nMasternodePaymentsStartBlock = 4010; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 4030;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 4200; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x0000047d24635e347be3aaaeb66c26be94901a2f962feccd4f95090191f208c1");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Momo: 1 day
        consensus.nPowTargetSpacing = 2.5 * 60; // Momo: 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 2; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 1;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1506556800; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1538092800; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1505692800; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1537228800; // Sep 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); 

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); 

        pchMessageStart[0] = 0x05;
        pchMessageStart[1] = 0xa5;
        pchMessageStart[2] = 0xb1;
        pchMessageStart[3] = 0x2a;
        vAlertPubKey = ParseHex("04d281fcbca43ccedf5f87f155a5d074d0394f7bea79632a9076a0dffce24bfe3527410e4fdf33d1c47d93339e6680f3019e53c724ba82bee1e7eb4c6be8337170");
        nDefaultPort = 17717;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1520438400,  1134259, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008035e7a698064d416bb5e8ccff0b6ded064885589611046c4ecd46b9c4a"));
        assert(genesis.hashMerkleRoot == uint256S("0x33c100a156d136f2b76297bbf9d6df385bcc9379b4777307271c27f8f4a4efbb"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Testnet Momo addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140);
        // Testnet Momo script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Testnet Momo BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Momo BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Testnet Momo BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (    0, uint256S("0x000008035e7a698064d416bb5e8ccff0b6ded064885589611046c4ecd46b9c4a")),

            1520438400, // * UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1.0         // * estimated number of transactions per day after checkpoint
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Momo: 1 day
        consensus.nPowTargetSpacing = 2.5 * 60; // Momo: 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nPowKGWHeight = 15200; // same as mainnet
        consensus.nPowDGWHeight = 34140; // same as mainnet
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb3;
        pchMessageStart[3] = 0xd1;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
        nDefaultPort = 19994;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1520438400, 1134259, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008035e7a698064d416bb5e8ccff0b6ded064885589611046c4ecd46b9c4a"));
        assert(genesis.hashMerkleRoot == uint256S("0x33c100a156d136f2b76297bbf9d6df385bcc9379b4777307271c27f8f4a4efbb"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x000008035e7a698064d416bb5e8ccff0b6ded064885589611046c4ecd46b9c4a")),
            1520438400,
            0,
            1.0
        };
        // Regtest Momo addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140);
        // Regtest Momo script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Regtest Momo BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest Momo BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest Momo BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

// place this code at the very bottom
void MineGenesisBlock(CBlock &genesis)
{
    arith_uint256 best = arith_uint256();
    int n=0;
    arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
    while (UintToArith256(genesis.GetHash()) > hashTarget) {
    arith_uint256 c=UintToArith256(genesis.GetHash());

    if(c < best || n==0)
    {
    best = c;
    n=1;
    printf("%s %s %s\n",genesis.GetHash().GetHex().c_str(),hashTarget.GetHex().c_str(),
    best.GetHex().c_str());
    }
    ++genesis.nNonce;
    if (genesis.nNonce == 0) { ++genesis.nTime; }
    }
    //printf("HASH IS: %s\n", UintToArith256(genesis.GetHash()).ToString().c_str());
    printf("Converting genesis hash to string: %s\n",genesis.ToString().c_str()); 
}