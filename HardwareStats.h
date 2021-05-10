
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