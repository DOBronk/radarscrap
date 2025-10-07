
#include "parser.h"

using namespace radarscraper;
using namespace simdjson;
using ::std::cout;
using ::std::map;

void Parser::parse(const std::string &json_value, ParserResults &p)
{
    parse(padded_string(json_value), p);
}

void Parser::parse(const padded_string &json_value, ParserResults &p)
{
    ondemand::parser parser;
    ondemand::document json = parser.iterate(json_value);

    for (ondemand::object road : json["roads"])
    {
        std::vector<segment> v_segments;
        std::string name;
        road["road"].get(name);

        for (ondemand::object json_seg : road["segments"])
        {
            segment seg;
            json_seg["start"].get(seg.from);
            json_seg["end"].get(seg.to);
            auto radars = json_seg.find_field("radars");

            if (!radars.error())
            {
                for (auto rads : radars)
                {
                    radar rad;
                    fill_radar(rad, rads);
                    seg.radars.push_back(rad);
                }
                p.totals.radars += seg.radars.size();
                v_segments.push_back(seg);
            }
        }

        if (v_segments.size() > 0)
        {
            p.totals.roads++;
            p.totals.segments += v_segments.size();
            p.collection.emplace(std::move(name), std::move(v_segments));
        }
    }
}

void Parser::fill_radar(radarscraper::radar &rad, simdjson_result<fallback::ondemand::value> &value)
{
    value["id"].get(rad.id);
    value["events"].at(0)["text"].get(rad.event);
    value["from"].get(rad.from);
    value["HM"].get(rad.hm);
    value["reason"].get(rad.reason);
    value["to"].get(rad.to);
    value["fromLoc"]["lat"].get(rad.positions.from.lat);
    value["fromLoc"]["lon"].get(rad.positions.from.lon);
    value["toLoc"]["lat"].get(rad.positions.to.lat);
    value["toLoc"]["lon"].get(rad.positions.to.lon);
    value["loc"]["lat"].get(rad.positions.position.lat);
    value["loc"]["lon"].get(rad.positions.position.lon);
}
