#include "database.h"

using ::std::cout;
using ::std::endl;
using ::std::vector;
using namespace ::mysqlx;
using namespace radarscraper;

bool DbManager::update_roadmap(ParserResults &results)
{
    try
    {
        ref_data data({.session = Session(config.db_host(), config.db_port(), config.db_user(), config.db_password())});
        data.roads_updated = data.roads_created = 0;
        data.session.sql(std::string("USE ").append(config.db_schema())).execute();
        data.session.startTransaction();
        for (auto &[name, value] : results.collection)
        {
            update_road(name, data);
            update_segments(value, data);
        }
        results.roads_inserted = data.roads_created;
        results.roads_updated = data.roads_updated;
        data.session.commit();
        data.session.close();
    }
    catch (std::exception Ex)
    {
        _error = Ex.what();
        return false;
    }
    return true;
}

bool DbManager::update_log(const ParserResults &results)
{
    Session db(config.db_host(), config.db_port(), config.db_user(), config.db_password());
    db.sql(std::string("USE ").append(config.db_schema())).execute();
    db.sql(sql_i_log).bind(results.runtime, results.totals.radars, results.totals.roads,
                           0, 0, 0, "None")
        .execute();

    db.close();
    return true;
}
inline void DbManager::update_road(const std::string &value, ref_data &data)
{
    data.road_id = data.session.sql(sql_i_roads).bind(value).execute().getAutoIncrementValue();
}

void DbManager::update_segments(const vector<segment> &value, ref_data &data)
{
    for (const segment &seg : value)
    {
        data.segment_id = data.session.sql(sql_i_segments).bind(seg.from, seg.to, data.road_id).execute().getAutoIncrementValue();
        update_radars(seg.radars, data);
    }
}

void DbManager::update_radars(const vector<radar> &value, ref_data &data)
{
    for (const radar &rad : value)
    {
        int id = update_positions(rad, data);
        int insorupd = data.session.sql(sql_i_radars).bind(rad.id, data.segment_id, id, rad.hm, rad.from, rad.to, rad.reason, rad.event).execute().getAffectedItemsCount();
        if (insorupd == 2)
        {
            data.roads_updated++;
        }
        else
        {
            data.roads_created++;
        }
    }
}

inline int DbManager::update_positions(const radar &value, ref_data &data)
{
    return data.session.sql(sql_i_positions).bind(data.segment_id, value.id, value.hm, value.positions.from.lat, value.positions.from.lon, value.positions.to.lat, value.positions.to.lon, value.positions.position.lat, value.positions.position.lon).execute().getAutoIncrementValue();
}
