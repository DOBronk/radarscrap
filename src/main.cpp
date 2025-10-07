#ifndef Analytics
#define Analytics
#endif

#include "common.h"
#include "scraper.h"
#include "database.h"
#include "summary.h"

using namespace std;
using namespace radarscraper;

int retries = 0;

int main(int argc, char *const argv[])
{
  handle_arguments(argc, argv, options, continious, print_summary, poll_interval);
  DbManager db;

  while (retries < MAX_RETRY)
  {
    ParserResults results;
    stopwatch.reset();
    scrape_result res;
    if ((res = Scraper::scrape(results)).success)
    {
      while (!db.update_roadmap(results))
        show_error("Error while committing to database", db.getError(), ++retries, RETRY_AFTER, MAX_RETRY);

      results.runtime = stopwatch.measure();

      try
      {
        db.update_log(results);
      }
      catch (exception ex)
      {
        show_error("Error while writing log to database", ex.what(), 0, 0, 0);
      }

      cout << results.totals.radars << " found on " << results.totals.roads << " roads. Radars updated: "
           << results.roads_updated << " inserted: " << results.roads_inserted << "\n";

      if (print_summary)
        Summary::printSummary(results.collection);

      if (!continious)
        break;

      retries = 0;
      sleep(poll_interval);
    }
    else
    {
      show_error("Error while scraping", res.error, ++retries, RETRY_AFTER, MAX_RETRY);
    }
  }

  return 0;
}
