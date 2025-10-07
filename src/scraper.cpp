#include <fstream>
#include "scraper.h"

const int MIN_DATA_LEN = 250; // If the returned response is less than this it can't be correct with our data

using namespace std;
using namespace radarscraper;

scrape_result Scraper::scrape(ParserResults &results)
{
    scrape_result res;
    simdjson::padded_string r;
    results.totals = {0, 0, 0};
    res.success = false;

    try
    {
        r = cpr::Get(cpr::Url{config.target()}, cpr::Header{{"accept", "application/json"}}).text;
    }
    catch (exception Ex)
    {
        res.error = Ex.what();
        return res;
    }

    if (r.size() < MIN_DATA_LEN)
    {
        res.error = "Server response is too small to be actual data. ";
        if (r.size() > 0)
        {
            res.error.append("Returned data: ").append(r.data());
        }
        return res;
    }

    try
    {
        Parser::parse(r, results);
    }
    catch (exception Ex)
    {
        res.error = Ex.what();
        return res;
    }

    res.success = true;
    return res;
}
