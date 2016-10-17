#include <iostream>
#include <math.h>
#include "tools.hpp"
#include "instruments.hpp"


using namespace std;

double Forward_Contract::black_scholes_price(stock S, double time)
  {
    double spot = S.get_spot();
    double rate = S.get_interest_rate()-S.get_dividend_rate();
    
    return spot - exp( -1 * rate * (expiry - time) ) * strike;
  }

double Call::black_scholes_price(stock S, double time)
  {
    double spot = S.get_spot();
    double rate = S.get_interest_rate()-S.get_dividend_rate();
    double vol = S.get_vol();
    double d1 = (log(spot/strike) + (rate + vol*vol/2.0) * (expiry-time))/(vol * sqrt(vol));
    double d2 = (log(spot/strike) + (rate - vol*vol/2.0) * (expiry-time))/(vol * sqrt(vol));
    
    return spot * Cum_Norm(d1)- exp( -1 * rate * (expiry - time) ) * strike * Cum_Norm(d2);
  }

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
