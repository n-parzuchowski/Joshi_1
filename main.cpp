// Vanilla Options pricing with Black-Scholes and Monte-Carlo
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "instruments.hpp"
using namespace std;

int main()
{

  stock ptx;
  Call opt;

  ptx.set_spot(100.0);
  ptx.set_vol(0.4);
  ptx.set_interest_rate(0.05);
  ptx.set_dividend_rate(0.0);

  cout << ptx.get_vol() << endl;
  
  opt.set_strike(100.0);
  opt.set_expiry(5.0);
  
  cout << opt.get_strike() << ' ' << opt.black_scholes_price(ptx,0.0) << endl;
  return 0;  
}
