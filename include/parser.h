#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "measure.h"
#include "simdjson.h"

namespace radarscraper
{
    typedef struct coordinates
    {
        float lat, lon;
    } loc;

    struct locations
    {
        loc from, to, position;
    };

    struct radar
    {
        long id;
        float hm;
        locations positions;
        std::string from, to, reason, event;
    };

    struct segment
    {
        std::string from, to;
        std::vector<radar> radars;
    };

    struct ParserTotals
    {
        int segments, radars, roads;
    };

    typedef std::map<std::string, std::vector<segment>> RadarCollection;

    struct ParserResults
    {
        RadarCollection collection;
        ParserTotals totals;
        int roads_inserted, roads_updated, runtime, exit_code;
        std::string errors;
    };

    class Parser
    {
    public:
        static void parse(const std::string &, ParserResults &);
        static void parse(const simdjson::padded_string &, ParserResults &);

    private:
        static void fill_radar(radarscraper::radar &, simdjson::simdjson_result<simdjson::fallback::ondemand::value> &);
        static segment get_segment(const auto &, auto);
    };
}
