// Vanilla Options pricing with Black-Scholes and Monte-Carlo

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "instruments.hpp"
#include <boost/format.hpp>
#include "tools.hpp"

using namespace std;
using boost::format; 


void print_mc_header() {
  // prints the header for the monte carlo calculation 
  cout << "\n==========================================================================================================" << endl;
  cout << "                                          MONTE CARLO CALCULATION " << endl;
  cout << "==========================================================================================================" << endl;
  cout << "    Stock          Call            Put           Forward        Digital Call   Digital Put    Riskless Bond" << endl; 
}
//==============================================================================
//============================================================================== 
void print_error_message(string error)
// usage statement
{
  cout << error << endl;
  cout << "Usage: run <SPOT> <STRIKE> <EXPIRY> <TIME_TODAY> <INT_RATE> <DIV_RATE> <VOL> <MC_dim> <steps>\n\n"; 
}
//==============================================================================
//==============================================================================       
int main(int argc, char* argv[])
{


  if (argc < 10){
    print_error_message("");
    return 0;
  }

  stock ptx;
  // Derivatives 
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
  int MCdim = atoi(argv[8]);
  int steps = atoi(argv[9]);
  
  
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
  fx << "strike  expiry  price \n";
  for ( int K = 10 ; K <= 200 ; K = K + 5)
    {
      C2.set_strike(K);
      for ( int T = 1 ; T <=10 ; T++ ) 
  	{
  	  C2.set_expiry(T);
  	  fx << format("%4i %4i %25.14f\n") % K % T % C2.price(ptx,current_time);	  
  	}
    }
  fx.close();
  
  int I = system("python plot_prices.py call_pricing.dat");

  // Monte Carlo Verification
  
  float * draw_vec;
  draw_vec = new float [MCdim]; 
  
  Gaussian_Random gen;
  gen.initialize();
  
  for (int i=0; i< MCdim ; i++){
    draw_vec[i] = gen.draw();
  }
  
  double Sx,Cx,Px,Fx,DCx,DPx,Bx;
  double Sy,Cy,Py,Fy,DCy,DPy,By;
  
  Sx=0;
  Cx=0;
  Px=0;
  Fx=0;
  DCx=0;
  DPx=0;
  Bx=0;

  double r = int_rate - div_rate;
  double T = expiry_time - current_time; 
  
  print_mc_header();
  for (int i=0; i < MCdim ; i++){  
    
    // New stock price
    double sT = spot_0 * exp( ( r - vol*vol/2.0)*T + vol * sqrt(T)*draw_vec[i]);
    ptx.set_spot(sT);
    
    // update relevant quantities 
    Sx = Sx + sT;
    Cx = Cx + C.pay(ptx,expiry_time);
    Px = Px + P.pay(ptx,expiry_time);
    Fx = Fx + F.pay(ptx,expiry_time);
    DCx = DCx + DC.pay(ptx,expiry_time);
    DPx = DPx + DP.pay(ptx,expiry_time);
    Bx = Bx + B.pay(ptx,expiry_time);

    // cute print statement to demonstrate Monte Carlo 
    if (i%(MCdim/1000) == 0) 
      {
	Sy = Sx / (i+1);
	Cy = Cx * exp(-r*T) / (i+1);
	Py = Px * exp(-r*T) / (i+1);
	Fy = Fx * exp(-r*T) / (i+1);
	DCy = DCx * exp(-r*T) / (i+1);
	DPy = DPx * exp(-r*T) / (i+1);
	By = Bx * exp(-r*T) / (i+1);    
    
	cout << format("%14.7f %14.7f %14.7f %14.7f %14.7f %14.7f %14.7f\r") % Sy % Cy % Py % Fy % DCy % DPy % By;
      }
  }

  double MC = MCdim;
  
  // Final quantities 
  Sy = Sx / MC;
  Cy = Cx * exp(-r*T) / MC;
  Py = Px * exp(-r*T) / MC;
  Fy = Fx * exp(-r*T) / MC;
  DCy = DCx * exp(-r*T) / MC;
  DPy = DPx * exp(-r*T) / MC;
  By = Bx * exp(-r*T) / MC;    

  cout << format("%14.7f %14.7f %14.7f %14.7f %14.7f %14.7f %14.7f") % Sy % Cy % Py % Fy % DCy % DPy % By << endl;

  delete[] draw_vec;



  // euler stepping monte carlo
 
  int paths = MCdim/steps; 

  // New draw_vec for a new monte carlo 
  draw_vec = new float [paths*steps]; 
  // walkers stores the position of several geometric brownian motions 
  float * walkers = new float[paths];
    
  for (int i=0; i< MCdim; i++){
    draw_vec[i] = gen.draw();
  }
  
  // initialize all walkers to the spot at strike time 
  for (int path = 0 ; path < paths; path++)
    {
      walkers[path]=spot_0;
    } 

  
  float dT = (expiry_time-current_time)/steps ;
  
  for (int step = 0; step < steps; step++){
    for ( int path = 0; path < paths; path++){
      
      walkers[path] = walkers[path] * exp( ( r - vol*vol/2.0)*dT + vol * sqrt(dT)*draw_vec[step * paths + path]);
	
    }
        
  }

  Sx=0;
  Cx=0;
  Px=0;
  Fx=0;
  DCx=0;
  DPx=0;
  Bx=0;

  // average over walkers 
  for ( int path = 0; path < paths; path++){
    ptx.set_spot(walkers[path]);

    Sx = Sx+ walkers[path];
    Cx = Cx + C.pay(ptx,expiry_time);
    Px = Px + P.pay(ptx,expiry_time);
    Fx = Fx + F.pay(ptx,expiry_time);
    DCx = DCx + DC.pay(ptx,expiry_time);
    DPx = DPx + DP.pay(ptx,expiry_time); 
    Bx = Bx + B.pay(ptx,expiry_time);
  }
  
  Sy = Sx / paths;
  Cy = Cx * exp(-r*T) / paths;
  Py = Px * exp(-r*T) / paths;
  Fy = Fx * exp(-r*T) / paths;
  DCy = DCx * exp(-r*T) / paths;
  DPy = DPx * exp(-r*T) / paths;
  By = Bx * exp(-r*T) / paths;    

  // output
  cout << format("%14.7f %14.7f %14.7f %14.7f %14.7f %14.7f %14.7f") % Sy % Cy % Py % Fy % DCy % DPy % By << endl;
      
  delete[] draw_vec;
  delete[] walkers;

    
  return 0;  
}
