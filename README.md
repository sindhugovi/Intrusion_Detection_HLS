An Intrusion Detection System (IDS) is a cybersecurity mechanism designed to monitor, detect, and respond to malicious activity or policy violations within a network or system. The goal is to identify signs of unauthorized access, cyberattacks, or abnormal behavior that could compromise data integrity, availability, or confidentiality.

In this project, a fully connected neural network (FCNN) is trained on network traffic data, where each input sample consists of 46 numerical features extracted from a packet or flow (e.g., duration, bytes sent, flags, protocol type).

The neural network outputs one of 34 predefined labels, each representing a class such as:

Normal traffic

Denial of Service (DoS)

Probe (information gathering)

Remote-to-Local (R2L)

User-to-Root (U2R)

And various specific attacks like smurf, teardrop, neptune, etc.

The WorkFlow
 1. Model Training
    1.1. Feature Preparation
The dataset consists of 46 numerical features extracted from network traffic data.

These features were normalized to a consistent scale (e.g., between -1 and 1) to improve model performance and stability.

Labels representing different types of network connections (normal or attack types) were encoded into numerical values.

    1.2. Dataset Splitting
The dataset was split into:

Training set: for learning model parameters.

Development (validation) set: for evaluating generalization performance during training.
    1.3. Neural Network Architecture
    The neural network used in this Intrusion Detection System (IDS) is a fully connected feedforward neural network with the following configuration:

Input Layer

Size: 46 nodes

Represents the 46 numerical features extracted from network traffic data.

Hidden Layer 1

128 neurons

Activation Function: ReLU (Rectified Linear Unit)

Hidden Layer 2

64 neurons

Activation Function: ReLU

Hidden Layer 3

32 neurons

Activation Function: ReLU

Output Layer

35 neurons (representing 35 distinct classes or types of network behavior, including normal and various attack categories)

No activation function used in this layer; the predicted class is determined by taking the argmax of the output values.

Each layer performs a fully connected linear transformation followed by a ReLU activation, except the output layer.

2. Quantization
Once the neural network model is trained using floating-point precision (e.g., float32), quantization is the next step to make the model hardware-friendly—especially for FPGAs where floating-point arithmetic is expensive in terms of power and resources.

3. Vivado HLS Implementation
   After training and quantizing your neural network model in Python, the next major step is converting the trained model into synthesizable C/C++ code and implementing it on an FPGA using Vivado HLS (High-Level Synthesis). This step bridges the gap between AI and hardware.

 3.1 Writing the C++ Inference Code
The HLS implementation of Neural Netwotk includes:

Input and Output Interface
A 46-element input vector (features of a network packet) and an output label (predicted class).

Layer Implementations
Each layer (fully connected + ReLU) is written as a function. For fixed-point implementation, int32_t data types and simulate scaling was used(e.g., using bit shifts instead of floating-point operations).

Optimization Directives (Pragmas)
HLS pragmas like #pragma HLS PIPELINE, #pragma HLS UNROLL, and #pragma HLS ARRAY_PARTITION were used.

3.2 Exporting Model Weights and Test Samples
Weights and biases are exported from Python as header files (W1.h, b1.h, etc.) in int32_t format.

A test sample is also exported in fixed-point for verification (test_sample.h).

These files are included in your HLS C++ code via #include.

3.3 C Simulation (csim)
 A neural_net_test.cpp file was written to:

Include test inputs

Call the neural_net function

Check the predicted output

Run C Simulation in Vivado HLS to verify functional correctness.

3.4 C/RTL Synthesis (csynth)
C Synthesis was performed, which converts the C++ code into synthesizable RTL (Verilog/VHDL). 

3.5 Interface Setup
AXI4-Sream interfaces was created for inputs (X[46]) and output (predicted_class). 
3.6 IP Export
Once synthesis is successful, the design was exported as IP
This IP can be used inside a Vivado block design for further integration (e.g., with a Zynq processor, DMA, or other logic).
