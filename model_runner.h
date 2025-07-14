#ifndef MODEL_RUNNER_H
#define MODEL_RUNNER_H

#include <vector>
#include <string>
#include <memory>
#include <onnxruntime_cxx_api.h>

class ModelRunner {
public:
    ModelRunner(const std::string &model_path);
    int predict(const std::vector<std::vector<float>> &sequence);

private:
    Ort::Env env;
    std::unique_ptr<Ort::Session> session;
};

#endif

