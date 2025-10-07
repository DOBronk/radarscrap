#include <iostream>

#include "common.h"
#include "summary.h"
#include "scraper.h"

using namespace std;
using namespace radarscraper;

const int MAX_RETRY = 200;
const int RETRY_AFTER = 5;
int retries = 0;

// ### START Command line parameters, see handle_arguments
bool continious = false;
bool print_summary = false;
int poll_interval = 60;

const string options[] = {"-c|--continious", "-s|--summary", "-i|--interval"};
// ### END Command line parameters

int main(int argc, char *const argv[])
{
  handle_arguments(argc, argv, options, continious, print_summary, poll_interval);
  DbManager db;

  while (retries < MAX_RETRY)
  {
    ParserResults results;
    Measure stopwatch;
    scrape_result res;
    if ((res = Scraper::scrape(results)).success)
    {
      while (!db.update_roadmap(results))
        showError("Error while committing to database", db.getError(), retries, RETRY_AFTER, MAX_RETRY);

      results.runtime = stopwatch.measure();

      try
      {
        db.update_log(results);
      }
      catch (exception ex)
      {
        showError("Error while writing log to database", ex.what(), 0, 0, 0);
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
      showError("Error while scraping", res.error, ++retries, RETRY_AFTER, MAX_RETRY);
    }
  }

  return 0;
}
