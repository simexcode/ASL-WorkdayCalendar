
#include "WorkdayCalendar.h"

namespace asl {
    using namespace std::chrono;
    using namespace date;

    bool WorkdayCalendar::isWorkDay(const date::year_month_day date) {

        //! a weekend is the most common and likely reason a given date would not be a workday
        //! since it's quick and cheap to check, we do that first and just return early 
        if (date::weekday(date) == date::sun || date::weekday(date) == date::sat)
            return false;

        //! if it's not a weekend, we have to check if this this is a holiday
        //! amt holiday dates are stored in two vectors, I believe a hashmap could be more performant
        //! but that would depend on the use case for this lib, like: 
        //! how many holidays are expected? are we going to have remove/add functionality?
        //! are we going to need to 'sort/specialize' holidays? e.g. different departments/people have different set's of holidays. 

        bool a = std::count(single_holidays.begin(), single_holidays.end(), date);
        bool b = std::count(recurring_holidays.begin(), recurring_holidays.end(), date.month() / date.day());
 
        return !(a || b);
    }

    void WorkdayCalendar::SetHoliday(const date::year_month_day date) {
        single_holidays.push_back(date);
    }

    void WorkdayCalendar::SetRecurringHoliday(const date::month_day date) {
        recurring_holidays.push_back(date);
    }

    void WorkdayCalendar::SetWorkdayStartAndStop(std::chrono::minutes start, std::chrono::minutes end) {
        //TODO: check that 'start' is before 'end'
        
        //! given the current info given I'm leaning towards throwing an exception
        //! if this were integrated in a larger system, I think I'd just clamp both
        //! 'start' and 'end' and essentially make a workday 0h long workday and just log a Warn.
        //! though, 0h or negative hour workdays are not guarded against, and would cause 
        //! WorkdayCalendar::GetWorkdayIncrement to stall, given the current implementation 
        
        workDayStart = start;
        workDayEnd = end;
        workDayLenght = end - start;
    }

    year_month_day_h_m WorkdayCalendar::GetWorkdayIncrement(year_month_day_h_m date, float incement) {
        int sign = incement >= 0 ? 1 : -1;
        auto workTime = workDayLenght * incement;
        auto daypoint = date::floor<date::days>(date); //convert y/m/d/h/s to sec since epic
       
        //get the date and time components
        auto time = std::chrono::minutes(date - daypoint);
        auto current = date::year_month_day(daypoint);
        auto remainingTime = workTime.count() * sign;

        //! if our start time is in the middle of a workday, we need to remember how far in to that day we are.
        auto remainder = isWorkDay(current) && time < workDayEnd && time > workDayStart ? time - workDayStart : 0h;
       
        while (remainingTime != 0) {

            //! if this is not a working day, just advance our day tracker and continue
            if (!isWorkDay(current)) {
                current = date::year_month_day(date::local_days(current) + date::days(sign));
                continue;
            }

            //! if this is a workday, but the day is over, advance the tracker and reset our timer to the beginning of the day
            if (time > workDayEnd && sign > 0) {
                current = date::year_month_day(date::local_days(current) + date::days(sign));
                time = workDayStart;
                continue;
            }

            //! if we get here there are only two options,
            //! 1. we have more then 1 full work day left to 'fulfill' -> subtract a full day and advance.
            //! 2. we are on the 'final' day -> figure out how far in to the workday we need to be. 
            if (remainingTime > workDayLenght.count()) {
                auto delta = workDayEnd - time;
                remainingTime -= workDayLenght.count();

            } else {
                time = sign > 0 ? workDayStart + std::chrono::minutes((int)remainingTime) : time = workDayEnd - std::chrono::minutes((int)remainingTime);
                time += remainder;
                remainingTime = 0;
            }

            //! check if we have found the day we need to be at, we could also break here instead of continuing
            if (remainingTime == 0)
                continue;

            //! if we get all the way here, just advance the tracker
            current = date::year_month_day(date::local_days(current) + date::days(sign));
        }

        //! return the day we 'counted' to and the time of day
        return local_days{current} + time;
    }
}