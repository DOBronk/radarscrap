#pragma once
#include <string>

#include "parser.h"

namespace radarscraper
{
    class Summary
    {
    public:
        static void printSummary(RadarCollection &results);
        static std::string getSummary(RadarCollection &results);

    private:
        static void segmentSummary(std::vector<segment> &, std::ostringstream &);
        static void radarSummary(std::vector<radar> &, std::ostringstream &);
    };
}