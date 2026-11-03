#ifndef ML_ALPHA_ENGINE_H
#define ML_ALPHA_ENGINE_H

#include <cmath>

class MLAlphaInferenceEngine {
public:
    // Simulates TensorRT/ONNX C++ Neural Network Forward Pass for price direction prediction
    inline double predictNextTickAlpha(double current_mid, double order_imbalance) {
        return (order_imbalance * 0.0018) + (std::sin(current_mid) * 0.0006);
    }
};

#endif // ML_ALPHA_ENGINE_H