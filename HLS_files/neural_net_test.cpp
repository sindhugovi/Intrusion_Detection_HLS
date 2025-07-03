#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <iostream>
#include "neural_net.h"  // where `neural_net_stream` is declared
#include "test_sample.h"
typedef ap_axiu<32, 0, 0, 0> stream32_t;
typedef ap_axiu<8, 0, 0, 0> stream8_t;

int main() {
    hls::stream<stream32_t> X_in;
    hls::stream<stream8_t> pred_out;


    // Send 46 inputs through the stream
    for (int i = 0; i < 46; i++) {
        stream32_t val;
        val.data = sample_input[i];
        val.last = (i == 45) ? 1 : 0;
        X_in.write(val);
    }

    // Call your HLS neural net
    neural_net(X_in, pred_out);

    // Read prediction result
    stream8_t result = pred_out.read();
    std::cout << "Predicted Class: " << (int)result.data << std::endl;

    return 0;
}
