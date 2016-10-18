#include <iostream>
#include <iomanip>
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

double Forward_Contract::payout(stock S, double time)
{
  double spot = S.get_spot();
  return spot - strike;
}

int Forward_Contract::print_name()
{
  cout << "Forward Contract" << endl;    
  return 0;
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

int Call::print_name()
{
  cout << "European Call" << endl;    
  return 0;
}



double Call::payout(stock S, double time)
{
  double spot = S.get_spot();
  if ( spot > strike ) 
    return spot - strike;
  else
    return 0.0; 
}

double Put::black_scholes_price(stock S, double time)
  {
    double spot = S.get_spot();
    double rate = S.get_interest_rate()-S.get_dividend_rate();
    double vol = S.get_vol();
    double d1 = -1*((log(spot/strike) + (rate + vol*vol/2.0) * (expiry-time))/(vol * sqrt(vol)));
    double d2 = -1*((log(spot/strike) + (rate - vol*vol/2.0) * (expiry-time))/(vol * sqrt(vol)));
    
    return  exp( -1 * rate * (expiry - time) ) * strike * Cum_Norm(d2) - spot * Cum_Norm(d1);
  }

double Put::payout(stock S, double time)
{
  double spot = S.get_spot();
  if ( spot < strike ) 
    return strike - spot;
  else
    return 0.0; 
}

int Put::print_name()
{
  cout << "European Put" << endl;    
  return 0;
}

double Digital_Call::black_scholes_price(stock S, double time)
  {
    double spot = S.get_spot();
    double rate = S.get_interest_rate()-S.get_dividend_rate();
    double vol = S.get_vol();
    double d2 = (log(spot/strike) + (rate - vol*vol/2.0) * (expiry-time))/(vol * sqrt(vol));
    
    return  exp( -1 * rate * (expiry - time) ) * Cum_Norm(d2);
  }

double Digital_Call::payout(stock S, double time)
{
  double spot = S.get_spot();
  if ( spot > strike ) 
    return 1.0;
  else
    return 0.0; 
}

int Digital_Call::print_name()
{
  cout << "Digital Call" << endl;    
  return 0;
}

double Digital_Put::black_scholes_price(stock S, double time)
  {
    double spot = S.get_spot();
    double rate = S.get_interest_rate()-S.get_dividend_rate();
    double vol = S.get_vol();
    double d2 = -1*((log(spot/strike) + (rate - vol*vol/2.0) * (expiry-time))/(vol * sqrt(vol)));
    
    return  exp( -1 * rate * (expiry - time) ) * Cum_Norm(d2);
  }

double Digital_Put::payout(stock S, double time)
{
  double spot = S.get_spot();
  if ( spot >= strike ) 
    return 0.0;
  else
    return 1.0; 
}

int Digital_Put::print_name()
{
  cout << "Digital Put" << endl;    
  return 0;
}


double ZeroCoupon_Bond::black_scholes_price(stock S, double time)
{
  double rate = S.get_interest_rate();
    
  return exp( -1 *rate * (expiry - time) );
}

double ZeroCoupon_Bond::payout(stock S, double time)
{
  double rate = S.get_interest_rate();
  return exp(-1*rate * (expiry-time));
}

int ZeroCoupon_Bond::print_name()
{
  cout << "Zero Coupon Riskless Bond" << endl;    
  return 0;
}


double derivative::price(stock S, double t)
{
  return black_scholes_price(S,t);
}


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


void derivative::print_details(stock S, double time)
{
  cout << "\n============================================" << endl;
  print_name();
  cout << "============================================" << endl;
  cout << "Strike: " << setprecision(12) << strike << endl;
  cout << "Time to expiry: " << setprecision(12) <<  expiry - time << endl;
  cout << "Current American payout: " << setprecision(12) <<  payout(S,time) << endl;
  cout << "Price: " << setprecision(12) <<  black_scholes_price(S,time) << endl;
  cout << "============================================" << endl;
  
}
