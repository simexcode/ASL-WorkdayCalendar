/*	!README!
*    -- WorkdayCalendar Exercise
*	 -- Mathias Langaas
* 
*  This solution makes heavy use of the date.h lib by Howard Hinnant
*  date.h is an extension/wrapper for chrono.h to make dealing with date-time easier
*  date.h was chosen because is been in use for about 10 years, is lightweight, fast
*  and as far as I have understood, a large part of Howard Hinnant date-time project is
*  being included in the c++20 standard of chrono.h.
* 
*  - https://github.com/HowardHinnant/date
* 
*	This project includes a lot of comments, comments with the '!' prefix are not comments I would have in
*	production code, they are only meant to help anyone evaluating this project to follow my thought process
*	and/or any ideas I have around the code.
*
*	The unitTest project will only link/build when running with visual studio
*   no cmake/premake are setup.
*/
#include <iostream>
#include "WorkdayCalendar.h"

using namespace std::chrono;
using namespace date;

int main() {
    asl::WorkdayCalendar calendar;

	calendar.SetWorkdayStartAndStop(8h + 0min, 16h + 0min);
	calendar.SetRecurringHoliday(17_d / May);
	calendar.SetHoliday(27_d / May / 2004);

	auto stateDate = local_days{ 24_d / May / 2004 } + 8h + 3min;
	auto result = calendar.GetWorkdayIncrement(stateDate, 12.782709f);

	// 24-05-2004 08:03 with the addition of 12.782709 working days is 10-06-2004 14:18 
	std::cout << result << std::endl;

	/* !
		- result is of type std::chrono::time_point<date::local_t, std::chrono::minutes>
		  and is not very usable, to do custom formatting, or further processing,
		  it can be deconstructed as such;

			//convert y/m/d/h/s to sec since epic (1st jan 1970)
			auto daypoint = date::floor<date::days>(result);

			//get the date and time components
			auto time =  date::make_time(result - daypoint);
			auto ymd = date::year_month_day(daypoint);
	*/
}