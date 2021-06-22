#pragma once
#include "Vendor/date.h"
#include <vector>
#include <time.h>

#define year_month_day_h_m std::chrono::time_point<date::local_t, std::chrono::minutes>

//asl -> AutoStoreLib
namespace asl {
	class WorkdayCalendar {
	private:
		std::chrono::minutes workDayStart, workDayEnd, workDayLenght;
		std::vector<date::year_month_day> single_holidays;
		std::vector<date::month_day> recurring_holidays;

		bool isWorkDay(const date::year_month_day date);

	public:
		void SetHoliday(const date::year_month_day date);
		void SetRecurringHoliday(const date::month_day date);
		void SetWorkdayStartAndStop(std::chrono::minutes start, std::chrono::minutes end);

		year_month_day_h_m GetWorkdayIncrement(year_month_day_h_m date, float incement);
	};
}