#pragma once
#include <cpr/cpr.h>
#include <string>

#include "common.h"
#include "parser.h"
#include "database.h"

namespace radarscraper
{
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