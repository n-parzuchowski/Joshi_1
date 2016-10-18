// Vanilla Options pricing with Black-Scholes and Monte-Carlo

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include "instruments.hpp"

using namespace std;



//==============================================================================
//============================================================================== 
void print_error_message(string error)
// usage statement
{
  cout << error << endl;
  cout << "Usage: run <SPOT> <STRIKE> <EXPIRY> <TIME_TODAY> <INT_RATE> <DIV_RATE> <VOL>\n\n"; 
}
//==============================================================================
//==============================================================================       
int main(int argc, char* argv[])
{


  if (argc < 8){
    print_error_message("");
    return 0;
  }

  stock ptx;
  Call C;
  Put P;
  Forward_Contract F;
  Digital_Call DC;
  Digital_Put DP;
  ZeroCoupon_Bond B;  

  double spot_0 = atof(argv[1]); // initial value of stock
  double strike = atof(argv[2]);
  double expiry_time = atof(argv[3]);
  double current_time = atof(argv[4]);
  double int_rate = atof(argv[5]);
  double div_rate = atof(argv[6]);
  double vol = atof(argv[7]);


  // set up stock 
  ptx.set_spot(spot_0);
  ptx.set_vol(vol);
  ptx.set_interest_rate(int_rate);
  ptx.set_dividend_rate(div_rate);

  
  // set up derivatives 
  C.set_strike(strike);
  C.set_expiry(expiry_time);
  C.print_details(ptx,current_time);

  P.set_strike(strike);
  P.set_expiry(expiry_time);
  P.print_details(ptx,current_time);

  F.set_strike(strike);
  F.set_expiry(expiry_time);
  F.print_details(ptx,current_time);

  B.set_strike(0.0); 
  B.set_expiry(expiry_time);
  B.print_details(ptx,current_time);

  DC.set_strike(strike);
  DC.set_expiry(expiry_time);
  DC.print_details(ptx,current_time);

  DP.set_strike(strike);
  DP.set_expiry(expiry_time);
  DP.print_details(ptx,current_time);
  

  // Test Black Scholes model for consistency 

  // Put Call Parity
  double C_minus_P= C.price(ptx,current_time)-P.price(ptx,current_time);

  if (abs( C_minus_P - F.price(ptx,current_time)) < 1e-10)
    cout << "\n\nPut call parity confirmed." << endl;
  else
    {
    cout << "\n\nPut call parity fails..." << endl;
    }

  // Digital Call and Put should sum to ZCB price
  double DC_plus_DP= DC.price(ptx,current_time)+DP.price(ptx,current_time);
  
  if (abs( DC_plus_DP - B.price(ptx,current_time)) < 1e-10)
    cout << "\n\nSum of digitals equals ZCB confirmed." << endl;
  else
    {
    cout << "\n\nSum of digitals not equal to ZCB..." << endl;
    }

  // Check no-arbitrage bounds of Call option S < C < S-K*exp(-rT) 
  if ((C.price(ptx,current_time) >= spot_0) ||
      (C.price(ptx,current_time) <= spot_0 - strike *exp(-int_rate * (expiry_time-current_time))))  
    {
    cout << "\nCall option price not within no-arbitrage bounds..." << endl;
    }
    else
    cout << "\nCall option price is within no-arbitrage bounds." << endl; 


  // Check properties of the call option price
  Call C2;

  // strike and time dependence 
  ofstream fx;
  fx.open("call_pricing.dat"); 
  fx << "# strike  expiry  price \n";
  for ( int K = 10 ; K <= 200 ; K = K + 5)
    {
      C2.set_strike(K);
      for ( int T = 1 ; T <=10 ; T++ ) 
	{
	  C2.set_expiry(T);
	  fx << K << " " << T << " "<< C2.price(ptx,current_time) << "\n";
	  
	}
    }
  fx.close();
	  
  
  return 0;  
}
