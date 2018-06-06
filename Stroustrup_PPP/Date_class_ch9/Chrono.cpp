//
//
// This is an example of a date class based on the example in Section 9.8 of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
//                Revised by Timofey Golubev June 2018

#include "Chrono.h"

namespace Chrono {

// member function definitions:

//------------------------------------------------------------------------------

Date::Date(int yy, Month mm, int dd)
    : y(yy), m(mm), d(dd)
{
    if (!is_date(yy,mm,dd)) throw Invalid();  //Invalid is a class with no members
}

//------------------------------------------------------------------------------

const Date& default_date()
{
    static const Date dd(2001,Date::Month::jan,1); // start of 21st century. static const so initialize only at 1st call, and unchanged.
    return dd;
}

//------------------------------------------------------------------------------
//default constructor: sets date = default_date(), defined above.
Date::Date()
    :y(default_date().year()),
     m(default_date().month()),
     d(default_date().day())
{
}

//------------------------------------------------------------------------------

void Date:: set_day(int n)
{
    d = n;
}

//------------------------------------------------------------------------------

void Date::set_month(int n)
{
    m = Month(n);
}

//------------------------------------------------------------------------------

void Date::set_year(int n)
{
    if (m==Month::feb && d==29 && !leapyear(y+n)) { // beware of leap years!
        m = Month::mar;        // use March 1 instead of February 29
        d = 1;
    }
    y+=n;
}

//------------------------------------------------------------------------------

// helper functions: (not members of Date class)

bool is_date(int y, Date::Month  m, int d)
{
    // assume that y (year) is valid

    if (d<=0) return false;            // d must be positive

    int days_in_month = 31;            // most months have 31 days

    switch (m) {
case Date::Month::feb:                        // the length of February varies
    days_in_month = (leapyear(y))?29:28;   //if leapyear, then 29, otherwise 28
    break;
case Date::Month::apr: case Date::Month::jun: case Date::Month::sep: case Date::Month::nov:
    days_in_month = 30;                // the rest have 30 days
    break;
    }

    if (days_in_month<d) return false;

    return true;
} 

//------------------------------------------------------------------------------
//check if is leapyear
bool leapyear(int y)
{
    //leap year is divisible by 4, but not 100. Exception: years divisible by 400 are leapyears.
    return (y%4==0 && y%100!=0) || y%400==0;
}

//------------------------------------------------------------------------------

bool operator==(const Date& a, const Date& b)
{
    return a.year()==b.year()
        && a.month()==b.month()
        && a.day()==b.day();
}

//------------------------------------------------------------------------------

bool operator!=(const Date& a, const Date& b)
{
    return !(a==b);  //== for dates is already defined above
}

//------------------------------------------------------------------------------


std::ostream& operator<<(std::ostream& os, const Date& d)
{
    return os << '(' << d.year()
              << ',' << int(d.month())     //cast month to int so can use <<
              << ',' << d.day() 
              << ')';
}

//------------------------------------------------------------------------------

std::istream& operator>>(std::istream& is, Date& dd)
{
    int y, m, d;
    char ch1, ch2, ch3, ch4;
    is >> ch1 >> y >> ch2 >> m >> ch3 >> d >> ch4;
    if (!is) return is;
    if (ch1!='(' || ch2!=',' || ch3!=',' || ch4!=')') { // oops: format error
        is.clear(std::ios_base::failbit);                    // set the fail bit
        return is;
    }
    dd = Date(y,Date::Month(m),d);     // update dd
    return is;
}

//------------------------------------------------------------------------------

enum class Day {
    sunday, monday, tuesday, wednesday, thursday, friday, saturday
};


} // end of Chrono


