#ifndef CONFIG
#define CONFIG

#include <map>
#include <string>

namespace Config
{
    static std::map<std::string, int> tier_score =
    {
        {"M", 44},
        {"D1", 40},
        {"D2", 36},
        {"D3", 34},
        {"D4", 32},
        {"E1", 30}, 
        {"E2", 28}, 
        {"E3", 26}, 
        {"E4", 24},  
        {"P1", 22},
        {"P2", 20},
        {"P3", 18},
        {"P4", 16},
        {"G1", 15},
        {"G2", 14},
        {"G3", 13},
        {"G4", 12},
        {"S1", 11},
        {"S2", 10},
        {"S3", 9},
        {"S4", 9},
        {"B1", 9},
        {"B2", 9},
        {"B3", 9},
        {"B4", 9},
        {"U", 0}
    };

    static std::map<std::string, std::string> line =
    {
        {"Å¾" , "TOP"},
        {"Á¤±Û", "JUG"},
        {"¹Ìµå", "MID"},
        {"¿øµô", "AD"},
        {"¼­Æý", "SUP"}
    };

    static std::string tier[26] =
    {
        "M", 
        "D1", "D2", "D3", "D4",
        "E1", "E2", "E3", "E4",
        "P1", "P2", "P3", "P4", 
        "G1", "G2", "G3", "G4", 
        "S1", "S2", "S3", "S4", 
        "B1", "B2", "B3", "B4",
        "U"
    };

}

#endif // !CONFIG
