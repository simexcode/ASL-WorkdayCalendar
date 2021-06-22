#include "pch.h"
#include "CppUnitTest.h"
#include "../WorkdayCalendar/WorkdayCalendar.h"

/* ! This is the first time I've used/implemented tests in c++
* There was little to no consideration as to the choice of testing lib
* I also did not spend much time on learning the lib outside of setting up basic tests
* I'd like to note that there are two major things I'd like to change/implement in the current setup
*
* 1. Use an Init_Module method for the:
*     " asl::WorkdayCalendar calendar;
*		calendar.SetWorkdayStartAndStop(8h + 0min, 16h + 0min);
*		calendar.SetRecurringHoliday(17_d / May);
*		calendar.SetHoliday(27_d / May / 2004); "
*	as I don't think it should be redefined in every test as it is now.
*	But with more extensive tests I'd like to test a wider range of staring/end times and holiday dates.
*
* 2. Print a useful message on an Assert failure, atm all you get in the test explorer is "Assert Failed"
*	 I'd much prefer something like "Expected: 14/5/2004 12:43  Got: 1/4/2004 10:00" in case of a failure
*
* Theses are thing I'd normally do in a testing framework with which I am familiar
*/


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::chrono;
using namespace date;

namespace WorkdayCalendarUnitTest
{
	TEST_CLASS(WorkdayCalendarUnitTest)
	{
	public:
			
		TEST_METHOD(TestNegativeIncrement_InWorkday){	
			asl::WorkdayCalendar calendar;

			calendar.SetWorkdayStartAndStop(8h + 0min, 16h + 0min);
			calendar.SetRecurringHoliday(17_d / May);
			calendar.SetHoliday(27_d / May / 2004);

			auto stateDate = local_days{ May / 24 / 2004 } + 18h + 5min;
			auto result = calendar.GetWorkdayIncrement(stateDate, -5.5f);

			//24-05-2004 18:05 with the addition of -5.5 working days is 14-05-2004 12:00
			Assert::IsTrue(result == local_days{ 14_d / May / 2004 } + 12h + 0min);
		}

		TEST_METHOD(TestNegativeIncrement_AfterWorkday) {
			asl::WorkdayCalendar calendar;

			calendar.SetWorkdayStartAndStop(8h + 0min, 16h + 0min);
			calendar.SetRecurringHoliday( 17_d / May );
			calendar.SetHoliday(27_d / May / 2004);

			auto stateDate = local_days{ 24_d / May / 2004 } + 18h + 3min;
			auto result = calendar.GetWorkdayIncrement(stateDate, -6.7470217f);

			//24-05-2004 18:03 with the addition of -6.7470217 working days is 13-05-2004 10:02
			Assert::IsTrue(result == local_days{ 13_d / May / 2004 } + 10h + 2min);
		}

		TEST_METHOD(TestLargeIncrement_AfterWorkday) {
			asl::WorkdayCalendar calendar;

			calendar.SetWorkdayStartAndStop(8h + 0min, 16h + 0min);
			calendar.SetRecurringHoliday(17_d / May);
			calendar.SetHoliday(27_d / May / 2004);

			auto stateDate = local_days{ 24_d / May / 2004 } + 19h + 3min;
			auto result = calendar.GetWorkdayIncrement(stateDate, 44.723656f);

			// 24-05-2004 19:03 with the addition of 44.723656 working days is 27-07-2004 13:47
			Assert::IsTrue(result == local_days{ 27_d / July / 2004 } + 13h + 47min);
		}

		TEST_METHOD(TestIncrement_InWorkday) {
			asl::WorkdayCalendar calendar;

			calendar.SetWorkdayStartAndStop(8h + 0min, 16h + 0min);
			calendar.SetRecurringHoliday(17_d / May);
			calendar.SetHoliday(27_d / May / 2004);

			auto stateDate = local_days{ 24_d / May / 2004 } + 8h + 3min;
			auto result = calendar.GetWorkdayIncrement(stateDate, 12.782709f);

			// 24-05-2004 08:03 with the addition of 12.782709 working days is 10-06-2004 14:18 
			Assert::IsTrue(result == local_days{ 10_d / June / 2004 } + 14h + 18min);
		}

		TEST_METHOD(TestIncrement_BeforeWorkday) {
			asl::WorkdayCalendar calendar;

			calendar.SetWorkdayStartAndStop(8h + 0min, 16h + 0min);
			calendar.SetRecurringHoliday(17_d / May);
			calendar.SetHoliday(27_d / May / 2004);

			auto stateDate = local_days{ 24_d / May / 2004 } + 7h + 3min;
			auto result = calendar.GetWorkdayIncrement(stateDate, 8.276628f);

			//24-05-2004 07:03 with addition of 8.276628 working days is 04-06-2004 10:12 
			Assert::IsTrue(result == local_days{ 4_d / June / 2004 } + 10h + 12min);
		}
	};
}
