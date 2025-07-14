#include "model_runner.h"
#include <onnxruntime_cxx_api.h>
#include <iostream>

ModelRunner::ModelRunner(const std::string &model_path)
    : env(ORT_LOGGING_LEVEL_WARNING, "gru"),
      session(nullptr)
{
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

    try {
        session = std::make_unique<Ort::Session>(env, model_path.c_str(), session_options);
        std::cout << "[ONNX] Model loaded: " << model_path << std::endl;
    } catch (const Ort::Exception &e) {
        std::cerr << "[ONNX] Failed to load model: " << e.what() << std::endl;
    }
}

int ModelRunner::predict(const std::vector<std::vector<float>> &sequence) {
    if (!session) {
        std::cerr << "[ONNX] Invalid session.\n";
        return 0;
    }

    const int N = sequence.size();
    const int F = sequence[0].size();
    std::vector<float> flat;
    for (const auto& vec : sequence)
        flat.insert(flat.end(), vec.begin(), vec.end());

    Ort::AllocatorWithDefaultOptions allocator;
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    std::array<int64_t, 3> input_shape{1, N, F};

    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info,
        flat.data(), flat.size(),
        input_shape.data(), input_shape.size());

    const char* input_names[] = {"input"};
    const char* output_names[] = {"output"};

    try {
        auto output = session->Run(
            Ort::RunOptions{nullptr},
            input_names, &input_tensor, 1,
            output_names, 1);

        float result = output[0].GetTensorMutableData<float>()[0];
        std::cout << "[ONNX] Prediction: " << result << std::endl;
        return static_cast<int>(result);
    } catch (const Ort::Exception &e) {
        std::cerr << "[ONNX] Inference error: " << e.what() << std::endl;
        return 0;
    }
}

