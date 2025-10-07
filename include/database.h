#pragma once
#include <mysqlx/xdevapi.h>
#include "common.h"
#include "parser.h"

namespace radarscraper
{
    struct ref_data
    {
        mysqlx::Session session;
        int road_id, segment_id, roads_updated, roads_created;
    };

    class DbManager
    {
    public:
        bool update_roadmap(ParserResults &results);
        bool update_log(const ParserResults &);
        const std::string getError() { return _error; };

    private:
        void update_road(const std::string &, ref_data &);
        void update_segments(const std::vector<segment> &, ref_data &);
        void update_radars(const std::vector<radar> &, ref_data &);
        int update_positions(const radar &, ref_data &);
        std::string _error;

        const char *sql_i_roads = "INSERT INTO roads (name) VALUES (?) ON DUPLICATE KEY UPDATE modified = CURRENT_TIMESTAMP()";
        const char *sql_i_segments = "INSERT INTO segments (start,end,road_id) VALUES (?,?,?) ON DUPLICATE KEY UPDATE modified = CURRENT_TIMESTAMP()";
        const char *sql_i_radars = "INSERT INTO radars (radar_id,segment_id,location_id, hm, start,end,reason,event) VALUES(?,?,?,?,?,?,?,?) ON DUPLICATE KEY UPDATE end_time = CURRENT_TIMESTAMP()";
        const char *sql_i_positions = "INSERT INTO locations (segment_id,radar_id,hm,from_lat,from_lon,to_lat,to_lon,lat,lon) VALUES (?,?,?,?,?,?,?,?,?) ON DUPLICATE KEY UPDATE hits = hits + 1";
        const char *sql_i_log = "INSERT INTO log (runtime, radar_count, road_count, created, updated, exit_code, errors, started_at, finished_at) VALUES (?,?,?,?,?,?,?,CURRENT_TIMESTAMP(),CURRENT_TIMESTAMP())";
    };
}