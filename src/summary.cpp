#include "summary.h"

using namespace std;
using namespace radarscraper;

void Summary::printSummary(RadarCollection &results)
{
    cout << getSummary(results);
}

string Summary::getSummary(RadarCollection &results)
{
    ostringstream str_summary;

    for (auto &[name, value] : results)
    {
        str_summary << "## Road " << name << ":" << endl;
        segmentSummary(value, str_summary);
    }

    return str_summary.str();
}

void Summary::segmentSummary(vector<segment> &segments, ostringstream &str_summary)
{
    for (segment &seg : segments)
    {
        str_summary << "Van: " << seg.from << " naar: " << seg.to << endl;
        radarSummary(seg.radars, str_summary);
        str_summary << endl;
    }
}

void Summary::radarSummary(vector<radar> &radars, ostringstream &str_summary)
{
    for (radar &rad : radars)
    {
        str_summary << "HM Paal: " << rad.hm << "\tvan " << rad.from << ", naar " << rad.to
                    << " -- Omschijving: " << rad.reason << " -- Type: " << rad.event
                    << " -- Kaartpositie: (lat: " << rad.positions.position.lat << " lon: "
                    << rad.positions.position.lon << ")" << endl;
    }
}
