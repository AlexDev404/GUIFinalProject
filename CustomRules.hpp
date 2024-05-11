#pragma once
#include <utility>

// Compares two pairs according to their value
bool compareAlbum(std::pair<int, int> i1, std::pair<int, int> i2) 
{ 
    return (i1.second > i2.second); // Checks to see if the second element of the pair is larger (orders in descending)
} 

// Then the rest of code can go in the other file (this file is just for organization)

// (Secret) To do list:
// In order of difficulty 1/3
/*
    1. Playlist: Add  (3)
                 Edit (3)
                 Delete (4)
    2. Translation (1)
    3. Adding different user restrictions (2)

    playlist first tho okay
    But when u double click to add edit delete there are word omg was i supposed to be puttin there stuff in tr()...?naw dont think so
    like u could but kinda alot of effort (in a way) T-T
*/