#include <hls_stream.h>
#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <stdint.h>
#include "W1.h"
#include "W2.h"
#include "W3.h"
#include "W4.h"
#include "b1.h"
#include "b2.h"
#include "b3.h"
#include "b4.h"

typedef int32_t data_t;
typedef ap_axiu<32, 0, 0, 0> stream32_t;
typedef ap_axiu<8, 0, 0, 0> stream8_t;

void relu_layer(data_t input[], data_t output[], int size) {
#pragma HLS INLINE
    for (int i = 0; i < size; i++) {
#pragma HLS UNROLL factor=4
        output[i] = (input[i] > 0) ? input[i] : 0;
    }
}

void fc1(const data_t input[46], data_t output[128]) {
#pragma HLS INLINE
    for (int i = 0; i < 128; i++) {
#pragma HLS PIPELINE
        int64_t sum = 0;
        for (int j = 0; j < 46; j++) {
            sum += (int64_t)input[j] * W1[i][j];
        }
        sum += b1[i];
        output[i] = (data_t)(sum >> 20);
    }
}

void fc2(const data_t input[128], data_t output[64]) {
#pragma HLS INLINE
    for (int i = 0; i < 64; i++) {
#pragma HLS PIPELINE
        int64_t sum = 0;
        for (int j = 0; j < 128; j++) {
            sum += (int64_t)input[j] * W2[i][j];
        }
        sum += b2[i];
        output[i] = (data_t)(sum >> 20);
    }
}

void fc3(const data_t input[64], data_t output[32]) {
#pragma HLS INLINE
    for (int i = 0; i < 32; i++) {
#pragma HLS PIPELINE
        int64_t sum = 0;
        for (int j = 0; j < 64; j++) {
            sum += (int64_t)input[j] * W3[i][j];
        }
        sum += b3[i];
        output[i] = (data_t)(sum >> 20);
    }
}

void fc4(const data_t input[32], data_t output[35]) {
#pragma HLS INLINE
    for (int i = 0; i < 35; i++) {
#pragma HLS PIPELINE
        int64_t sum = 0;
        for (int j = 0; j < 32; j++) {
            sum += (int64_t)input[j] * W4[i][j];
        }
        sum += b4[i];
        output[i] = (data_t)(sum >> 20);
    }
}

int8_t max_index(data_t input[], int size) {
    data_t max_val = input[0];
    int idx = 0;
    for (int i = 1; i < size; i++) {
        if (input[i] > max_val) {
            max_val = input[i];
            idx = i;
        }
    }
    return idx;
}

// Top-level HLS function with AXI-Stream interface
void neural_net(
    hls::stream<stream32_t>& X_in,
    hls::stream<stream8_t>& pred_out
) {
#pragma HLS INTERFACE axis port=X_in
#pragma HLS INTERFACE axis port=pred_out
#pragma HLS INTERFACE ap_ctrl_none port=return

    data_t X[46];
    data_t a1[128], r1[128];
    data_t a2[64],  r2[64];
    data_t a3[32],  r3[32];
    data_t a4[35];

#pragma HLS ARRAY_PARTITION variable=a1 complete
#pragma HLS ARRAY_PARTITION variable=r1 complete
#pragma HLS ARRAY_PARTITION variable=a2 complete
#pragma HLS ARRAY_PARTITION variable=r2 complete
#pragma HLS ARRAY_PARTITION variable=a3 complete
#pragma HLS ARRAY_PARTITION variable=r3 complete
#pragma HLS ARRAY_PARTITION variable=a4 complete

    // Read 46 input values from AXI stream
    for (int i = 0; i < 46; i++) {
#pragma HLS PIPELINE II=1
        stream32_t tmp = X_in.read();
        X[i] = tmp.data;
    }

    // Forward pass
    fc1(X, a1);
    relu_layer(a1, r1, 128);

    fc2(r1, a2);
    relu_layer(a2, r2, 64);

    fc3(r2, a3);
    relu_layer(a3, r3, 32);

    fc4(r3, a4);
    int8_t predicted_class = max_index(a4, 35);

    // Send result to AXI output
    stream8_t out_val;
    out_val.data = predicted_class;
    out_val.last = 1;
    pred_out.write(out_val);
}
