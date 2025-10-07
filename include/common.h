#pragma once
// Common header file with most default includes and functions that will be used by most files

// Our dev environment is Linux so we set UNIX
#define UNIX

#include <iostream>
#include <string>
#include <map>
#include <vector>

#ifdef UNIX
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "measure.h"
#include "config.h"

static radarscraper::Config config("radarscraper.conf");

struct ScrapeReport
{
    int runtime, total_roads, total_radars, total_segments, exit_code;
    std::string errors;
};

// Some handy inline functions
inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

/**
 * @brief  Handles error messages
 * @param message  Brief details (eg. Error while commiting to database)
 * @param what  The exception what message or a custom error
 * @param retries  The amount of retries for the erroneous function
 * @param t_sleep  The amount of time to sleep (0 = off)
 * @param max_retires  The maximum amount of retries before program gets killed
 * @param exit_code  The exit code to use when terminate conditions are met
 *  Prints the current error and will either sleep or terminate program when
 *  retries is equal or higher than max retries.
 */
static void showError(const std::string &message, const std::string &what, const int retries, const int t_sleep = 0, const int max_retries = 200, const int exit_code = -1)
{
    std::cout << message;
    if (!message.ends_with("."))
        std::cout << ".";
    std::cout << " Error: " << message << std::endl;

    if (max_retries > 0 && retries >= max_retries)
        exit(exit_code);

    if (t_sleep > 0)
    {
        std::cout << "Trying again in " << t_sleep << "s. Retries: " << retries << "/" << max_retries << std::endl;
        sleep(t_sleep);
    }
    else
    {
        std::cout << "Skipping error." << std::endl;
    }
}

/**
 * @brief  Searches for aliases of parameters delimited by '|'
 * @param search  The value to look for
 * @param str  The delimited list to look through
 *  Function used by handle_arguments
 */
static bool alias_exists(std::string search, const std::string &str)
{
    size_t has_more = str.find('|');

    if (has_more == std::string::npos)
        return search == str;

    int pos = 0;

    while (has_more != std::string::npos)
    {
        if (str.substr(pos, has_more) == search)
            return true;

        pos += has_more + 1;
        has_more = str.find('|', pos);
    }

    return str.substr(pos) == search;
}

// TODO: implement some nice error handling

/**
 * @brief  Handles parameters from main
 * @param argc  Argument count from main
 * @param argv[]  Char *const array of parameters
 * @param options[]  String array with parameters, delimit aliasses by '|' character
 * @param vals {...}  The values to be assigned, in order of options list
 *  Make sure to match array size of options with parameters and in correct order.
 *  Booleans will get toggled, int/double/string will be set from space seperated parameter.
 */
template <typename... T>
static void handle_arguments(int argc, char *const argv[], const std::string *options, T &...vals)
{
    if (argc < 2)
        return;

    int i = 0;

    ([&]
     {     
        for(int x = 1; x < argc; x++)
        {
            if (alias_exists(std::string(argv[x]), options[i]))
            {
                char t_arg = typeid(vals).name()[0];
                
                if (x+1 >= argc && t_arg != 'b')
                {
                    std::ostringstream streamstr;
                    streamstr << vals;
                    showError("Parameter error", std::string("No value after ").append(streamstr.str()), 0, 0, 0);
                }

                switch (t_arg) {
                case 'b':
                  if constexpr ( std::is_same<T, bool>::value ) {
                    vals = !vals;
                  }
                    break;
                case 'i':
                    vals = std::atoi(argv[++x]);
                    break;
                case 'd':
                    vals = std::atof(argv[++x]);
                    break;
                default:
                    if constexpr ( std::is_same<T, std::string>::value ) {
                        vals = std::string(argv[++x]);
                    }
                    break;
                };
            }
        }
    i++; }(), ...);
};
