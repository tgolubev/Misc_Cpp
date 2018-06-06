// This is tests the implementation of the Date class based on the example in Section 9.8 of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
//                Revised by Timofey Golubev June 2018
//
// Creates a date and test some of the functions in namespace Chrono to manipulate dates.

#include <iostream>
#include "Chrono.h"

using namespace std;

int main()
try
{
    Chrono::Date today{2018, Chrono::Date::Month::jun, 5};        //initialize a date
    Chrono::Date tomorrow = today;
    tomorrow.set_day(today.day() + 1);                     //+1 to today to get tomorrow

    cout << "today " << today <<endl;                  //<< was overloaded for Date objects
    cout << "tomorrow " << tomorrow << endl;

    return 0;

}
catch (Chrono::Date::Invalid&) {
    cerr << "error: Invalid date\n";
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n";
    return 2;
}
