#include <iostream>

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
  virtual double black_scholes_price(stock , double)=0;
  virtual double payout(stock,double)=0;
  virtual int print_name(void)=0;
  double price(stock, double);
  double pay(stock, double); 
  double get_strike(void);
  double get_expiry(void);
  void set_strike(double);
  void set_expiry(double);
  void print_details(stock,double); 
protected:
  double strike,expiry;
};


// the following classes extend "derivative" 
class Forward_Contract: public derivative
{
public: 
  double black_scholes_price(stock S, double time);
  double payout(stock S, double time);
  int print_name(void);
};

class Call: public derivative
{
public: 
  double black_scholes_price(stock S, double time);
  double payout(stock S, double time);  
  int print_name(void);
};

class Put: public derivative
{
  double black_scholes_price(stock S, double time);
  double payout(stock S, double time);  
  int print_name(void);
};

class Digital_Call: public derivative
{
  double black_scholes_price(stock S, double time);
  double payout(stock S, double time);  
  int print_name(void);
};

class Digital_Put: public derivative
{
  double black_scholes_price(stock S, double time);
  double payout(stock S, double time);  
  int print_name(void);
};

class ZeroCoupon_Bond: public derivative
{
  double black_scholes_price(stock S, double time);
  double payout(stock S, double time);  
  int print_name(void);
};
