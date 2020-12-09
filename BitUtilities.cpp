#include "BitUtilities.h"

    /// <summary>
    /// FROM: From https://www.geeksforgeeks.org/extract-k-bits-given-position-number/ 
    /// Function to extract k bits from p position 
    /// and returns the extracted value as integer 
    /// So, for p = 0 and k = 3, starting from bit 0, we take the next 3 bits on the left
    /// including bit 0. So, for 1010 0010, we return 1010 0[010] : the part in brackets
    /// For p = 1 and k = 3, we get 1010 [001]0.
    /// For p = 3 and k = 3, we get 10[10 0]010.
    /// For p = 1 and k = 2, we get 10100 [01]0.
    /// </summary>
    int BitUtilities::ExtractBits(int number, int k, int p) {
        return (((1 << k) - 1) & (number >> (p))); 
    }

    /// <summary>
    ///
    /// </summary>
    int BitUtilities::CreateBitMasking(int startBit, int endBit)
    {
	    int mask = 0;
	    for (int i = startBit; i <= endBit; ++i) {
		mask |= 1 << i;
    	}
	    return mask;
    }   