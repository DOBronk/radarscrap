#pragma once
#include <chrono>

namespace radarscraper
{
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::nanoseconds nano;

    class Measure
    {
    public:
        Measure()
        {
            reset();
        }

        void reset()
        {
            s_time = Clock::now();
        }

        // Return value in ms as a float with a max precision of 6 (ns / 10^6)
        const float measure()
        {
            return (float)std::chrono::duration_cast<nano>(Clock::now() - s_time).count() / 1e+6;
        }

    private:
        Clock::time_point s_time;
    };
}