// Vanilla Options pricing with Black-Scholes and Monte-Carlo
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "instruments.h"
#include "tools.h"
using namespace std;

int main()
{

  stock* ptx = new stock();
  Forward_Contract* opt = new Forward_Contract();

  ptx->set_spot(100.0);
  ptx->set_vol(0.4);
  ptx->set_interest_rate(0.05);
  ptx->set_dividend_rate(0.0);
      
  //  opt->set_strike(100.0);
  //  opt->set_expiry(500.0);
  
    
  return 0;  
}
