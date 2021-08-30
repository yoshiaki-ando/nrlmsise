#include <iostream>
#include <cmath>
#include <fstream>

#include "nrlmsise-00.h"

using Ap_array = struct ap_array;
using NRLMSISE_Input = struct nrlmsise_input;
using NRLMSISE_Flag = struct nrlmsise_flags;
using NRLMSISE_Output = struct nrlmsise_output;

#define BOLTZMANN_CONSTANT 1.38064852e-23

int main(void){
  constexpr double kB = BOLTZMANN_CONSTANT;
  Ap_array Xp;
  NRLMSISE_Input Input;
  NRLMSISE_Flag Flag;
  NRLMSISE_Output Output;

  for(int i = 0; i < 7; i++){
    Xp.a[i] = 100.0;
  }

  Flag.switches[0] = 0;
  for(int i = 1; i < 24; i++){
    Flag.switches[i] = 1;
  }

  Input.year = 2019;
  Input.doy = 172;
  Input.sec = 43200;
  Input.g_lat = 50;
  Input.g_long = 135;
  Input.lst = 21.0;
  Input.f107A = 150.;
  Input.f107 = 150.;
  Input.ap = 4.0;
  Input.ap_a = &Xp;

  Input.alt = 32.5;
  gtd7(&Input, &Flag, &Output);
  for(int i = 0; i < 9; i++){
    std::cout << i << " " << Output.d[i] << std::endl;
  }
  for(int i = 0; i < 2; i++){
    std::cout << i << " " << Output.t[i] << std::endl;
  }
  exit(0);

  std::ofstream ofs("msise.dat");
  for(int i = 0; i < 110; i++){
    Input.alt = double(i);
    gtd7(&Input, &Flag, &Output);

    double n = 0.0;
    for(int j = 0; j < 5; j++){
      n += Output.d[j];
    }
    n += Output.d[6] + Output.d[7]; /* [cm^{-3}] */

    double p = (n*1e6) * kB * Output.t[1];

    ofs << i << " "
        << Output.t[1] << " "
        << n << " "
        << p << std::endl;
  }
  ofs.close();
}
