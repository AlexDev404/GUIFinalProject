#pragma once
#include <utility>
#include <algorithm> 
#include <cctype>
#include <locale>

class Utility {

public:
// trim from both ends (in place)
static void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
};



// trim from start (copying)
static std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
};

// trim from end (copying)
static std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
};

// trim from both ends (copying)
static std::string trim_copy(std::string s) {
    trim(s);
    return s;
};

// trim from start (in place)
static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
};

// trim from end (in place)
static void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
};

// Compares two pairs according to their value
static bool compareAlbum(std::pair<int, int> i1, std::pair<int, int> i2) 
{ 
    return (i1.second > i2.second); // Checks to see if the second element of the pair is larger (orders in descending)
} 


};
// Then the rest of code can go in the other file (this file is just for organization)

// (Secret) To do list:
// In order of difficulty 1/3
/*
    1. Playlist: Add  (3)
                 Edit (3)
                 ~Delete (4)~
    2. Translation (1)
    3. Adding different user restrictions (2)
    
    */