#pragma once
#include <utility>

// Compares two pairs according to their value
bool compareAlbum(std::pair<int, int> i1, std::pair<int, int> i2) 
{ 
    return (i1.second > i2.second); // Checks to see if the second element of the pair is larger (orders in descending)
} 

// Then the rest of code can go in the other file (this file is just for organization)
