#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef Analytics
#include <chrono>
#include <future>
#endif

#include "config.h"

#ifdef Analytics
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::nanoseconds nano;
class Stopwatch
{
public:
    Stopwatch()
    {
        reset();
    }
    /**
     * @brief Restart measuring from this point
     */
    inline void reset()
    {
        s_time = Clock::now();
    }
    /**
     * @brief Get start time as time_point
     */
    inline Clock::time_point begin()
    {
        return s_time;
    }
    /**
     * @brief Get current time as time_point
     */
    inline Clock::time_point end()
    {
        return Clock::now();
    }

    /**
     * @brief Get current time since start or last reset
     * @return (double) value in ms of time passed
     */
    inline const double measure()
    {
        return (double)std::chrono::duration_cast<nano>(Clock::now() - s_time).count() / 1e+6;
    }

private:
    Clock::time_point s_time;
};
#endif

static const int MAX_RETRY = 200;
static const int RETRY_AFTER = 5;

// *** START Command line parameters, see handle_arguments
inline const std::string options[] = {"-c|--continious", "-s|--summary", "-i|--interval"};
inline bool continious = false;
inline bool print_summary = false;
inline int poll_interval = 60;

/**
 * @brief  Handles error messages and terminates program if retries exhausted
 * @param message  Brief details (eg. Error while commiting to database)
 * @param what  The exception what message or a custom error
 * @param retries  Amount of sequental error retries
 * @param t_sleep  (optional) The amount of time to sleep (0 = off)
 * @param max_retires  (optional) Maximum amount of retries before termination
 * @param exit_code  (optional) The exit code to use when terminate retries exhausted
 *
 **/
static void show_error(const std::string &message, const std::string &what, const int retries, const int t_sleep = 0, const int max_retries = 200, const int exit_code = -1)
{
    std::cout << message << message.ends_with(".") ? "" : ".";
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
        if (alias_exists(std::string(argv[x]), options[i]))
            {
                char t_arg = typeid(vals).name()[0];
                
                if (x+1 >= argc && t_arg != 'b')
                {
                    std::ostringstream streamstr;
                    streamstr << vals;
                    show_error("Parameter error", std::string("No value after ").append(streamstr.str()), 0, 0, 0);
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
    i++; }(), ...);
};

// Useful tools
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
