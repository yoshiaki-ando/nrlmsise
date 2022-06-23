#include <iostream>
#include <cmath>
#include <fstream>

#include "nrlmsise-00.h"

/* NRL-MSISE00の入出力用 */
using Ap_array = struct ap_array;
using NRLMSISE_Input = struct nrlmsise_input;
using NRLMSISE_Flag = struct nrlmsise_flags;
using NRLMSISE_Output = struct nrlmsise_output;

constexpr double BOLTZMANN_CONSTANT { 1.38064852e-23 };

int main(void){
  constexpr double kB { BOLTZMANN_CONSTANT };
  Ap_array Xp;
  NRLMSISE_Input Input;
  NRLMSISE_Flag Flag;
  NRLMSISE_Output Output;

  /* 以下のパラメタは特に気にしなくて良い */
  /* 磁気指数。データはNASAあたりのWebサイトにあるが、標準的な値を使う */
  for(int i = 0; i < 7; i++){
    Xp.a[i] = 100.0;
  }

  /* 計算する機能を設定するフラグ */
  Flag.switches[0] = 0; /* 0 -> kg, m を使用, 1 -> g, cm を使用 */
  for(int i = 1; i < 24; i++){
    Flag.switches[i] = 1;
  }
  /* 以上のパラメタは特に気にしなくて良い */

  Input.year = 2019; /* 年は機能していないと思う */
  Input.doy = 172; /* Day of Year; 1月1日を1、2月1日を32、...、12月31日を365とする(除く閏年) */
  Input.sec = 43200; /* 0時0分を 0、23時59分を86400-1 */
  Input.g_lat = 50; /* [deg] 緯度 */
  Input.g_long = 135;/* [deg] 経度、西経は負 */
  Input.lst = 21.0; /* [hours] local apparent solar time, lst = sec/3600 + g_long/15 にする */
  Input.f107A = 150.; /* F10.7フラックスの81日平均。標準的な値 */
  Input.f107 = 150.; /* 当日のF10.7フラックス。標準的な値 */
  Input.ap = 4.0; /* 磁気指数。標準的な値 */
  Input.ap_a = &Xp; /* 同上 */

  Input.alt = 32.5; /* [km] 高度 */
  gtd7(&Input, &Flag, &Output);
  for(int i = 0; i < 9; i++){
    std::cout << i << " " << Output.d[i] << std::endl;
  }
  for(int i = 0; i < 2; i++){
    std::cout << i << " " << Output.t[i] << std::endl;
  }

  std::ofstream ofs("msise.dat");
  for(int i = 0; i < 110; i++){
    Input.alt = double(i);
    gtd7(&Input, &Flag, &Output);

    /*
     * 出力 (指定された高度)
     *      d[0] - He 数密度 [cm^{-3}]
     *      d[1] - O 数密度 [cm^{-3}]
     *      d[2] - N2 数密度 [cm^{-3}]
     *      d[3] - O2 数密度 [cm^{-3}]
     *      d[4] - Ar 数密度 [cm^{-3}]                       
     *      d[5] - 全質量密度 [g/cm^3] [includes d[8] in td7d]
     *      d[6] - H 数密度 [cm^{-3}]
     *      d[7] - N 数密度 [cm^{-3}]
     *      d[8] - Anomalous oxygen 数密度 [cm^{-3}]
     *      t[0] - [K] EXOSPHERIC TEMPERATURE (外気圏温度)
     *      t[1] - [K] 温度
     */

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
