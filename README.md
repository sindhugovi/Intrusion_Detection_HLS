🔐 Intrusion Detection System (IDS) on FPGA using Neural Networks
An Intrusion Detection System (IDS) is a cybersecurity mechanism designed to monitor, detect, and respond to malicious activity or policy violations within a network or system. The goal is to identify signs of unauthorized access, cyberattacks, or abnormal behavior that could compromise data integrity, availability, or confidentiality.

🚀 Project Overview
This project presents a hardware-efficient Intrusion Detection System (IDS) implemented on an FPGA using a fully connected neural network (FCNN) trained on preprocessed network traffic features.

The trained model is quantized to int32 fixed-point.

Implemented using Vivado HLS.

Optimized for real-time detection on resource-constrained edge devices.

🧠 Neural Network Details
Each input sample consists of 46 numerical features (e.g., duration, bytes sent, flags, protocol type).

🔍 Classification Output
The model outputs one of 35 predefined labels, such as:

✅ Normal Traffic

💥 Denial of Service (DoS)

🕵️‍♂️ Probe (Information Gathering)

📩 Remote-to-Local (R2L)

🧑‍💻 User-to-Root (U2R)

🔻 Specific attacks like smurf, teardrop, neptune, etc.

📊 Workflow
🛠️ 1. Model Training
⚙️ 1.1 Feature Preparation
Extracted 46 numerical features from the dataset.

Applied normalization to scale values between -1 and 1.

Labels were encoded as numeric values.

✂️ 1.2 Dataset Splitting
Training Set: For learning model weights.

Validation Set: To test generalization performance.

🧱 1.3 Neural Network Architecture
Layer	Neurons	Activation
Input Layer	46	—
Hidden Layer 1	128	ReLU
Hidden Layer 2	64	ReLU
Hidden Layer 3	32	ReLU
Output Layer	35	Argmax

🧮 2. Quantization
After training in float32, all weights and biases were quantized to int32 using a scaling factor (e.g., 1e6).

This enables fixed-point arithmetic on FPGA.

Output activations are scaled back using bit shifts (e.g., >> 20).

🧰 3. Vivado HLS Implementation
After training and quantization, the neural network was implemented in C++ using Vivado HLS, and deployed to FPGA as a hardware IP core.

🧾 3.1 Writing the C++ Inference Code
Written using fixed-point int32_t types.

Layers (FC + ReLU) were coded as functions with efficient loop pipelining.

Used int64_t for accumulations to prevent overflow.

ReLU and argmax functions implemented manually.

🗃️ 3.2 Exporting Model Parameters
Weights and biases saved as header files:

W1.h, b1.h, ..., W4.h, b4.h

Test inputs exported as test_sample.h

⚙️ 3.3 C Simulation (csim)
Used neural_net_test.cpp for simulation.

Verified correct prediction of known test input.

💡 3.4 C/RTL Synthesis (csynth)
HLS converted C++ code to Verilog/VHDL.

Timing and resource utilization analyzed.

🔌 3.5 Interface Setup
Implemented AXI4-Stream interface for:

X[46] input

predicted_class output

📦 3.6 IP Export
Exported the synthesized module as a Vivado IP block.
