#include "unistd.h"
#include "getopt.h"
#include "Vvscale_verilator_top.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#define VCD_PATH_LENGTH 256

int main(int argc, char **argv, char **env) {
  
  int c;
  int digit_optind = 0;
  char vcdfile[VCD_PATH_LENGTH];

  while (1) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      {"vcdfile", required_argument, 0,  0 },
      {0,         0,                 0,  0 }
    };

    c = getopt_long(argc, argv, "",
                    long_options, &option_index);
    if (c == -1)
      break;
    
    switch (c) {
    case 0:
      if (optarg)
        strncpy(vcdfile,optarg,VCD_PATH_LENGTH);
      break;
    default:
      break;
    }
  }

  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  Vvscale_verilator_top* verilator_top = new Vvscale_verilator_top;
  verilator_top->trace(tfp, 99); // requires explicit max levels param
  tfp->open("obj_dir/t_trace_ena_cc/simx.vcd");
  vluint64_t main_time = 0;
  while (!Verilated::gotFinish()) {
    verilator_top->eval();
    tfp->dump(main_time);
    main_time++;
  }
  delete verilator_top;
  exit(0);
}
