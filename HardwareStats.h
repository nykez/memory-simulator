//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		HardwareStats.h
//	Description:    Hold statistical information on memory components.
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Wednesday, December 6 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef HARDWARE_STATS_H
#define HARDWARE_STATS_H

/// <summary>
/// Holds statistical information on memory components. 
/// (Hits/Misses/Rate)
/// </summary>
struct HardwareStats {
    int hits = 0;           // number of successes
    int misses = 0;         // number of fails
    double hitrate = 0.0;   // hits / (hits + misses)
    
    ///<summary>
    /// Default Constructor
    ///</summary>
    HardwareStats() {
        
    }

    ///<summary>
    /// Constructor
    ///</summary>
    /// <param name="hitCount">number of hits</param>
    /// <param name="missCount">number of misses</param>
    HardwareStats(int hitCount, int missCount) {
        hits = hitCount;
        misses = missCount;
        hitrate = (double)(hits) / (double)(hits + misses);
    }
};

#endif //HARDWARE_STATS_H