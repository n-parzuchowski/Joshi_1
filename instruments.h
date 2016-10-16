#include <iostream>
#include "tools.h"

// A class for the underlying stock 
class stock{
  double rate,dividend_rate,value,vol;  
public:
  double get_interest_rate(void);
  double get_dividend_rate(void);
  double get_spot(void);
  double get_vol(void);
  double forward_price(double);
  void set_interest_rate(double);
  void set_dividend_rate(double);
  void set_spot(double);
  void set_vol(double); 
};

// this is an abstract class for
// general derivatives 
class derivative{
public:
  virtual double black_scholes_price(stock, double);
  double get_strike(void);
  double get_expiry(void);
  void set_strike(double);
  void set_expiry(double);
protected:
  double strike,expiry;  
};


// the following classes extend "derivative" 
class Forward_Contract: public derivative
{
public: 
  double black_scholes_price(stock S, double time);  
};

class Call: public derivative
{
public: 
  double black_scholes_price(stock S, double time);
};

/* class Put: public derivative */
/* { */
/* }; */

/* class Digital_Call: public derivative */
/* { */
/* }; */

/* class Digital_Put: public derivative */
/* { */
/* }; */

/* class ZeroCoupon_Bond: public derivative */
/* { */
/* }; */


/* double derivative::get_strike(); */

/* double derivative::get_expiry(); */

/* double stock::get_interest_rate(); */

/* double stock::get_dividend_rate(); */

/* double stock::get_spot(); */

/* double stock::get_forward_price(double time); */

/* void derivative::set_strike(double K); */

/* void derivative::set_expiry(double T); */

/* void stock::set_interest_rate(double r); */

/* void stock::set_dividend_rate(double d); */

/* void stock::set_spot(double S); */

/* void stock::set_vol(double sig); */

/* double stock::get_vol(); */
