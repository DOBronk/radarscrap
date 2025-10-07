#pragma once
#include <fstream>
#include <cpr/cpr.h>
#include <string>

#include "common.h"
#include "parser.h"

namespace radarscraper
{
    struct ScrapeReport
    {
        int runtime, total_roads, total_radars, total_segments, exit_code;
        std::string errors;
    };

    struct scrape_result
    {
        bool success;
        std::string error;
    };

    class Scraper
    {
    public:
        static scrape_result scrape(ParserResults &);
    };
}