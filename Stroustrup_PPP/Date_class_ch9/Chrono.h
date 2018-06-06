//
// This is example code from Chapter 9.8 "The Date class" of 
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
//             Revised by Timofey Golubev June 2018

//------------------------------------------------------------------------------

#ifndef CHRONO_H
#define CHRONO_H

#include <iostream>
#include <istream>

namespace Chrono {


class Date {
public:
    enum class Month {
        jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec         //rest of months auto filled increasing +1 ints
    };

    class Invalid { };               //throw as an exception

    Date(int y, Month m, int d);       // check for valid date and initialize
    Date();                            // default constructor
    // the default copy operations are fine

    // non-modifying operations: const ensures that fnc can't modify
    int   day()   const { return d; }
    Month month() const { return m; }
    int   year()  const { return y; }

    // modifying operations:
    void set_day(int n);
    void set_month(int n);
    void set_year(int n);
private:
    int   y;
    Month m;
    int   d;
};

//------------------------------------------------------------------------------

bool is_date(int y, Date::Month m, int d); // true for valid date

//------------------------------------------------------------------------------

bool leapyear(int y);                  // true if y is a leap year

//------------------------------------------------------------------------------

bool operator==(const Date& a, const Date& b);
bool operator!=(const Date& a, const Date& b);

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Date& d);
std::istream& operator>>(std::istream& is, Date& dd);

//------------------------------------------------------------------------------

} // Chrono

#endif // CHRONO_H
