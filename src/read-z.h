for (int j = 5; j <= 10; j++)
    {   
        LogPrintf("INSIDE BLOCK: %d\n", j);
        CBlockIndex* pindex = chainActive[j];
        //boost::this_thread::interruption_point();
        //uiInterface.ShowProgress(_("Verifying blocks..."), std::max(1, std::min(99, (int)(((double)(chainActive.Height() - pindex->nHeight)) / (double)nCheckDepth * (nCheckLevel >= 4 ? 50 : 100)))));
        //if (pindex->nHeight < chainActive.Height()-nCheckDepth)
            //break;
        CBlock block;
        // CValidationState state;
        // auto verifier = libzcash::ProofVerifier::Enabled();
        // check level 0: read from disk
        if (!ReadBlockFromDisk(block, pindex))
            return error("VerifyDB(): *** ReadBlockFromDisk failed at %d, hash=%s", pindex->nHeight, pindex->GetBlockHash().ToString());
        
        for (int i = block.vtx.size() - 1; i >= 0; i--) {
            LogPrintf("INSIDE TRANSACTION: %d\n", i);
            const CTransaction &tx = block.vtx[i];

            // unspend nullifiers
            BOOST_FOREACH(const JSDescription &joinsplit, tx.vjoinsplit) {
                //commitments
                LogPrintf("vpub_old: %lld\n", joinsplit.vpub_old);
                LogPrintf("vpub_new: %lld\n", joinsplit.vpub_new);
                LogPrintf("anchor: %s\n", joinsplit.anchor.GetHex());
                LogPrintf("ephemeralKey: %s\n", joinsplit.ephemeralKey.GetHex());
                LogPrintf("randomSeed: %s\n", joinsplit.randomSeed.GetHex());
            
                BOOST_FOREACH(const uint256 &cm, joinsplit.commitments) {
                    LogPrintf("cm: %s\n", cm.GetHex());
                }
                //nullifiers
                BOOST_FOREACH(const uint256 &nf, joinsplit.nullifiers) {
                    LogPrintf("nf: %s\n", nf.GetHex());
                }
            }
            LogPrintf("\n");
        }
        LogPrintf("\n");

        // check level 1: verify block validity
        // if (!CheckBlock(block, state, verifier))
        //     return error("VerifyDB(): *** found bad block at %d, hash=%s\n", pindex->nHeight, pindex->GetBlockHash().ToString());
        // check level 2: verify undo validity
        // if (nCheckLevel >= 2 && pindex) {
        //     CBlockUndo undo;
        //     CDiskBlockPos pos = pindex->GetUndoPos();
        //     if (!pos.IsNull()) {
        //         if (!UndoReadFromDisk(undo, pos, pindex->pprev->GetBlockHash()))
        //             return error("VerifyDB(): *** found bad undo data at %d, hash=%s\n", pindex->nHeight, pindex->GetBlockHash().ToString());
        //     }
        // }
        // check level 3: check for inconsistencies during memory-only disconnect of tip blocks
        // if (nCheckLevel >= 3 && pindex == pindexState && (coins.DynamicMemoryUsage() + pcoinsTip->DynamicMemoryUsage()) <= nCoinCacheUsage) {
        //     bool fClean = true;
        //     if (!DisconnectBlock(block, state, pindex, coins, &fClean))
        //         return error("VerifyDB(): *** irrecoverable inconsistency in block data at %d, hash=%s", pindex->nHeight, pindex->GetBlockHash().ToString());
        //     pindexState = pindex->pprev;
        //     if (!fClean) {
        //         nGoodTransactions = 0;
        //         pindexFailure = pindex;
        //     } else
        //         nGoodTransactions += block.vtx.size();
        // }
        // if (ShutdownRequested())
        //     return true;
    }