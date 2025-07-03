# 🔐 Intrusion Detection System on FPGA using Neural Networks

This project presents a hardware-efficient **Intrusion Detection System (IDS)** implemented on an **FPGA** using a fully connected neural network (FCNN) trained on preprocessed network traffic features. The model is quantized to **fixed-point int32** and deployed using **Vivado HLS**, making it suitable for **real-time, resource-constrained environments** such as edge devices.

---

## 🛡️ What is an Intrusion Detection System (IDS)?

An **IDS** is a cybersecurity mechanism that monitors and analyzes network traffic to detect suspicious activities or policy violations. Its primary goal is to identify signs of:

- 🚫 Unauthorized access  
- 🐛 Cyberattacks  
- 📉 Abnormal network behavior  

This project uses machine learning to automatically detect and classify these threats.

---

## 🧠 Neural Network Overview

- **Input Features**: 46 numerical features extracted from network packets (e.g., bytes sent, protocol type).
- **Output**: One of **35 classes**, including:
  - ✅ Normal traffic
  - 💥 DoS (Denial of Service)
  - 🕵️ Probe (Scanning)
  - 📩 R2L (Remote-to-Local)
  - 🧑‍💻 U2R (User-to-Root)
  - 🔻 Specific attacks like smurf, teardrop, neptune, etc.

---

## 📊 Workflow

### 1️⃣ Model Training

#### 📌 Feature Preparation
- Normalized 46 features to the range [-1, 1].
- Encoded labels into integers representing different traffic types.

#### ✂️ Dataset Splitting
- Training set for learning parameters.
- Validation set for measuring performance.

#### 🧱 Network Architecture

| Layer           | Neurons | Activation |
|-----------------|---------|------------|
| Input           | 46      | —          |
| Hidden Layer 1  | 128     | ReLU       |
| Hidden Layer 2  | 64      | ReLU       |
| Hidden Layer 3  | 32      | ReLU       |
| Output          | 35      | Argmax     |

---

### 2️⃣ Quantization

To make the model hardware-efficient:

- Trained model weights and biases in `float32` were **quantized to int32** using a scale factor (e.g., `1e6`).
- Used **bit shifts** (e.g., `>> 20`) to simulate rescaling during inference.
- Exported as C header files (`W1.h`, `b1.h`, etc.)

---

### 3️⃣ Vivado HLS Implementation

#### 🧾 C++ Implementation
- Implemented each layer as a function using `int32_t` and `int64_t` (for accumulation).
- ReLU and argmax functions included.
- Used HLS pragmas:
  - `#pragma HLS PIPELINE`
  - `#pragma HLS UNROLL`
  - `#pragma HLS ARRAY_PARTITION`

#### 🧪 C Simulation (csim)
- Verified model output using fixed-point test samples (`test_sample.h`).

#### 🔄 RTL Synthesis (csynth)
- Converted C++ to synthesizable Verilog/VHDL.
- Analyzed area, latency, and performance.

#### 🔌 AXI Interface
- Configured `AXI4-Stream` interfaces for:
  - Input: `X[46]`
  - Output: `predicted_class`

#### 📦 IP Export
- Exported as a Vivado IP block for integration with:
  - Zynq Processing System
  - AXI Interconnect
 

---

## ✅ Outcome

A real-time, FPGA-deployable neural network for intrusion detection using fixed-point arithmetic, optimized for edge hardware using Vivado HLS.

---

