#include <iostream>
#include "tools.h"
#include <math.h>

using namespace std;

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

//this is an abstract class for
//general derivatives 
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
  double black_scholes_price(stock S, double time)
  {
    double spot = S.get_spot();
    double rate = S.get_interest_rate()-S.get_dividend_rate();
    
    return spot - exp( -1 * rate * (expiry - time) ) * strike;
  }
  
};

class Call: public derivative
{
public: 
  double black_scholes_price(stock S, double time)
  {
    double spot = S.get_spot();
    double rate = S.get_interest_rate()-S.get_dividend_rate();
    double vol = S.get_vol();
    double d1 = (log(spot/strike) + (rate + vol*vol/2.0) * (expiry-time))/(vol * sqrt(vol));
    double d2 = (log(spot/strike) + (rate - vol*vol/2.0) * (expiry-time))/(vol * sqrt(vol));
    
    return spot * Cum_Norm(d1)- exp( -1 * rate * (expiry - time) ) * strike * Cum_Norm(d2);
  }

};

// class Put: public derivative
// {
// };

// class Digital_Call: public derivative
// {
// };

// class Digital_Put: public derivative
// {
// };

// class ZeroCoupon_Bond: public derivative
// {
// };

double derivative::get_strike()
{
  return strike;
}
double derivative::get_expiry()
{
  return expiry;
}
double stock::get_interest_rate()
{
  return rate;
}
double stock::get_dividend_rate()
{
  return dividend_rate;
}
double stock::get_spot()
{
  return value;
}
double stock::forward_price(double time)
{
  return value * exp( (rate-dividend_rate) * time );
}
void derivative::set_strike(double K)
{
  strike = K;
}
void derivative::set_expiry(double T)
{
  expiry = T;
}
void stock::set_interest_rate(double r)
{
  rate = r;
}
void stock::set_dividend_rate(double d)
{
  dividend_rate = d;
}
void stock::set_spot(double S)
{
  value = S;
}
void stock::set_vol(double sig)
{
  vol = sig;
}
double stock::get_vol()
{
  return vol;
}
