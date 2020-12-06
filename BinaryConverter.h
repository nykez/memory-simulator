//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		BinaryConverter.h
//	Description:    Converter methods for the bit arrays 
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Tuesday, December 4 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include <math.h>
#include <string>
#include <list>
namespace BinaryConverter
{
    /// <summary>
    /// Converts an array of bits to an integer
    /// </summary>
    /// <param name="bitArr">Array of bits</param>
    /// <returns>integer representing the value of the bit array</returns>
    int ToBinaryInt(std::vector<int> bitArr)
    {
        int bin = 0; 
        int col = bitArr.size() - 1;
        int power = 0;
        for(; col >= 0; col--, power++)
        {
            bin += (bitArr[col] * std::pow(2, power));
        }
        return bin;
    }

    std::vector<int> ToBitArray(int binaryInt) 
    {
        if(binaryInt == 0) return {0}; 
        
        std::vector<int> bitArray;
        std::vector<int>::iterator it;

        while(binaryInt > 0)
        {
            it = bitArray.begin();
            bitArray.insert(it, binaryInt % 2);
            binaryInt /= 2;
        }
        bitArray.shrink_to_fit();
        return bitArray;        
    }

} // namespace BinaryConverter
