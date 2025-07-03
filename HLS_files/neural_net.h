#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include <stdint.h>

typedef int32_t data_t; // Must match quantized weight/bias precision

void neural_net(hls::stream<ap_axiu<32,0,0,0>> &X_in,
                       hls::stream<ap_axiu<8,0,0,0>> &pred_out);


#endif
