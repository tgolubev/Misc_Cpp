/*
   Simple calculator

    Revision history:

        Revised by Timofey Golubev June 2018
        Revised by Bjarne Stroustrup May 2007
        Revised by Bjarne Stroustrup August 2006
        Revised by Bjarne Stroustrup August 2004
        Originally written by Bjarne Stroustrup Spring 2004.


    This program implements a basic expression calculator.
    Input from cin; output to cout.

   The operations +,-,*,/, and % (% is available with integers only) are available as well as using variables to symplify the expressions.
   To define a variable use the keyword 'let' followed by the variable definition:
   i.e. let x = 5;

   Predefined names: pi, e, and k = 1000 are available for use.

   The calculator also understands e notation: i.e.  5e2 = 500

   Functions available: sqrt() and pow(x,i) = x^i. x and i can be any expression.

   Variable names must start with a letter and can have numbers, but no special symbols.
   End each expression with ; followed by [Enter] to print the results. Use 'quit' to quit.

   If you have made a typo, you will get an error message. To enter another expression, first enter
   ; followed by [Enter].


    The grammar for input is:

    Calculation:
        Statement
        Print
        Quit
        Calculation Statement

    Statement:
        Declaration
        Expression

    Declaration:
        "let" Name "=" Expression

    Print:
        ;

    Quit:
        q

    Expression:
        Term
        Expression + Term
        Expression - Term
    Term:
        Primary
        Term * Primary
        Term / Primary
        Term % Primary
    Primary:
        Number
        Name
        sqrt ( Expression )
        pow(Expression, int)
        ( Expression )
        - Primary
        + Primary
    Number:
        floating-point-literal


        Input comes from cin through the Token_stream called ts.
*/

#include "std_lib_facilities.h"

//-----------------------------------------------------------------------------------
const char number = '8';        // t.kind==number means that t is a number Token
const char quit   = 'q';        // t.kind==quit means that t is a quit Token
const char print  = ';';        // t.kind==print means that t is a print Token
const char name   = 'a';        // name token
const char let    = 'L';        // declaration token
const char square_root   = 's';  //square root token
const char power = 'p';          //power token
const string declkey = "let";   // declaration keyword
const string quitkey = "quit";  //quit keyword
const string prompt  = "> ";
const string result  = "= ";    // used to indicate that what follows is a result
const string sqrt_key    = "sqrt";  //square root keyword
const string power_key = "pow";    //power keyword

//-------------------------------------------------------------------------------

struct Token {
    char kind;          //kind of token
    double value;       //value for numbers
    string name;        //name for variables
    Token(char ch)             :kind{ch}, value{0} { }     //for operators/letters
    Token(char ch, double val) :kind{ch}, value{val} { }   //for numbers
    Token(char ch, string n)   :kind{ch}, name{n} { }     //for variables
};

//------------------------------------------------------------------------------------

class Token_stream {
public:
    Token_stream() :  full(false), buffer(0) { }   //reads from cin
    Token get();                                   //get a Token
    void unget(Token t);    //put the got Token back. Token t is stored in buffer.
    void ignore(char c);   //discard tokens up to and including a particular char. Used for clean_up_mess() after an error occurs
private:
    bool full;            //is there a Token in the buffer?
    Token buffer;         //keep Token put back using unget() here
};

//--------------------------------------------------------------------------------------------

struct Variable {
    string name;
    double value;
    Variable(string n, double v) :name(n), value(v) { }
};

//------------------------------------------------------------------------------------------


double expression(Token_stream &ts);


vector<Variable> var_names;   //store names of variables

//----------------------------------------------------------------------------------

void Token_stream::unget(Token t)  //Put the got Token back. Token t is stored in buffer.
{
     if (full) error("putback() into a full buffer");
     buffer=t;   //copy t to the buffer
     full=true;  //indicate that buffer is full
}

//--------------------------------------------------------------------------------------

void set_value(string s, double d)           //set the value of variable named s to d
{
    for (int i = 0; i<=var_names.size(); ++i)
        if (var_names[i].name == s) {
            var_names[i].value = d;
            return;
        }
    error("set: undefined name ",s);
}

//---------------------------------------------------------------------------------

bool is_declared(string s)      //check is variable is already declared (already in var_names vector)
{
    for (int i = 0; i<var_names.size(); ++i)
        if (var_names[i].name == s) return true;
    return false;
}

//-------------------------------------------------------------------------------

Token Token_stream::get()  //read characters from cin and compose a Token
{
    if (full) { full=false; return buffer; }  //check if there is a Token in the buffer, in that case return it.
	char ch;
    cin >> ch;        //recall that >> skips whitespace
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
    case print:
	case '=':
    case ',':  //for pow
        return Token(ch);  //each character represents itself
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
    {	cin.unget();  //put character back into input stream. Note: not the same unget() as we defined (this is member fnc of cin)
		double val;
		cin >> val;
		return Token(number,val);
	}
	default:
        if (isalpha(ch)) {         //ifalpha checks if ch is a letter. If is a letter, then it could be part of a variable name, so start creating a string.
			string s;
			s += ch;
            while(cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) s+=ch;  //while next characters are also letters or #'s, add them to string. NOTE: cin.get, does not skip whitespace.
            cin.unget();                           //put next character back into input stream, if it's not the above.1
            if (s == declkey) return Token(let);   //variable declaration keyword
            if (s == quitkey) return Token(quit);  //return token corresponding to quit
            if (s == sqrt_key) return Token(square_root);
            if (s == power_key) return Token(power);
            //check if there's an '=' after variable name
            char next_char;
            cin >> next_char;
            if(next_char == '=' && is_declared(s)){
                Token t = get();
                set_value(s, t.value);      // if have = after variable name and variable is already declared, then means is an assignment--> call expression
                return t;
            }
            cin.unget();       //return the char back to cin stream so can be read elsewhere
            return Token(name,s);
		}
		error("Bad token");
	}
}

//---------------------------------------------------------------------------------------------------

void Token_stream::ignore(char c)  //used for clean_up_mess() after an error occurs
{
    //first look in buffer:
    if (full && c==buffer.kind) {    //if find the c kind of Token there, then stop ignoring input
		full = false;
		return;
	}
	full = false;

    // now search input: ignore, until find a c kind of Token
	char ch;
	while (cin>>ch)
		if (ch==c) return;
}

//---------------------------------------------------------------------------------------

double get_value(string s)       //return the value of variable named s
{
    for (int i = 0; i<var_names.size(); ++i)
        if (var_names[i].name == s) return var_names[i].value;
	error("get: undefined name ",s);
}

//-----------------------------------------------------------------------------
//deal with numbers and parentheses
double primary(Token_stream &ts)
{
	Token t = ts.get();
	switch (t.kind) {
    case '(':                    // handle '(' expression ')'
    {	double d = expression(ts);
		t = ts.get();
        if (t.kind != ')') error("')' expected");
        return d;
	}
	case '-':
        return - primary(ts);       //unitary - (i.e. if expression starts with -5)
    case '+':
        return primary(ts);         //unitary +
	case number:
        return t.value;            //return number's value
	case name:
        return get_value(t.name);  //return variable's value
    case square_root:
    {    t = ts.get();
        if (t.kind != '(') error("'(' expected");   //after sqrt should have '('
        double e = expression(ts);
        //check for closing )
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        if(e < 0) error("Can't take sqrt of negative number");
        return sqrt(e);      //sqrt() is from STL library cmath defined in header
    }
    case power:
    {   t = ts.get();
        if (t.kind != '(') error("'(' expected");   //after pow should have '('
        double p = expression(ts);
        //check for ,
        t = ts.get();
        if (t.kind != ',') error("',' expected. Recall format for power is pow(x,i) is x^i.");
        double i = expression(ts);
        //check for closing )
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return pow(p,i);
    }
	default:
		error("primary expected");
	}
}

//---------------------------------------------------------------------------------

//deal with *,/, and %. Will be operated on after all primaries have been evaluated.
double term(Token_stream &ts)
{
    double left = primary(ts);
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '*':
            left *= primary(ts);
			break;
		case '/':
        {	double d = primary(ts);
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
        case '%':
                {
                    int i1 = narrow_cast<int>(left);  //% requires int operators. cast to int.
                    int i2 = narrow_cast<int>(term(ts));
                    if (i2 == 0) error("%: divide by zero");
                    left = i1%i2;
                    break;
                }
        default:              //if char is non of the chars that would make this a term
            ts.unget(t);      //put t back into  token stream so other functions can read it
			return left;
		}
	}
}

//------------------------------------------------------------------------------------
//deal with + and - (will be operated on, after all terms have been evaluated)
double expression(Token_stream &ts)
{
    double left = term(ts);   //read and evaluate a term
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '+':
            left += term(ts);
			break;
		case '-':
            left -= term(ts);
			break;
		default:
            ts.unget(t);       //if next character is not + or -, then previous thing is a term which we return.
                               //And put the next char back into token stream since it's not something that can form an expression.
			return left;
		}
	}
}

//----------------------------------------------------------------------------------
//handle: name =  expression
//declare a variable called "name" with the initial value "expression"

double declaration(Token_stream &ts)
{
	Token t = ts.get();
    if (t.kind != name) error ("name expected in declaration");
	string name = t.name;
	if (is_declared(name)) error(name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " ,name);
    double d = expression(ts);
    var_names.push_back(Variable(name,d));
	return d;
}

//---------------------------------------------------------------------

double statement(Token_stream &ts)                  //recognizes if is a declaration or expression
{
	Token t = ts.get();
	switch(t.kind) {
	case let:
        return declaration(ts);
	default:
		ts.unget(t);
        return expression(ts);
	}
}

//---------------------------------------------------------------------

void clean_up_mess(Token_stream &ts)
{
	ts.ignore(print);
}

//------------------------------------------------------------------


void calculate(Token_stream &ts)
{
    while(true) try {       //while(true) will continue in the loop until reaches a break or return.
		cout << prompt;
		Token t = ts.get();
        while (t.kind == print) t=ts.get();  //first eat all "print" statements
        if (t.kind == quit) return;         //quit
        ts.unget(t);
        cout << result << statement(ts) << endl;
	}
	catch(runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        cerr << "Please re-enter your expression. If don't see the '>' prompt,  type ; followed by [Enter]" << endl;
        clean_up_mess(ts);
	}
}

//----------------------------------------------------------------------

int main()

	try {
        cout << "Welcome to our simple calculator." <<endl;
        cout <<"Please enter expressions followed by  ; and [Enter] key to print the result. Scientific e notation (i.e. 1e2 = 100) can be used" <<endl;
        cout <<"Operators +,-,*,/, %(for int) are avalable. Variables can be defined using 'let', i.e. let x = 5;" << endl;
        cout << "Predefined types pi, e, and k = 1000 and functions sqrt(), pow(x,i) = x^i (x and i can be any expression) are also available." << endl;
        cout << "To quit, use 'quit' followed by [Enter] key" << endl;
        // predefine names:
        var_names.push_back(Variable("pi",3.1415926535));
        var_names.push_back(Variable("e", 2.7182818284));
        var_names.push_back(Variable("k", 1000));
        Token_stream ts;
        calculate(ts);
		return 0;
	}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
		return 1;
	}
	catch (...) {
		cerr << "exception\n";
		return 2;
	}

