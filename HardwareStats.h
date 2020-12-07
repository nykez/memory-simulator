/// NAME: HardwareStats.h
/// PURPOSE: Hold statistical information on memory components.
/// AUTHOR: Harrison Lee Pollitte
/// CREATED: 12/6/2020
/// NOTES: Used by OutputDisplayer.h for displaying stats to console.
/// *      Used by MemoryController.h to track information on components.
/// *

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