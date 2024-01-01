#include "ort_session.h"

#include "absl/log/log.h"

const OrtApiBase* ONNXSession::gApiBase = OrtGetApiBase();

ONNXSession::ONNXSession(int output_size):output_dims_size_(output_size) { api_ = gApiBase->GetApi(14); }
bool ONNXSession::loadModel(const std::string& path) {
  // 初始化ONNX Runtime环境
  OrtStatus* status =
      api_->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "ONNXInference", &env_);
  if (status != nullptr) {
    LOG(WARNING) << "Failed to create ONNX Runtime environment:"
                 << api_->GetErrorMessage(status);
    api_->ReleaseStatus(status);
    return false;
  }

  // 加载模型
  OrtStatus* load_status =
      api_->CreateSession(env_, path.c_str(), nullptr, &session_);
  if (load_status != nullptr) {
    LOG(WARNING) << "Failed to load ONNX model:"
                 << api_->GetErrorMessage(load_status);
    api_->ReleaseStatus(load_status);
    api_->ReleaseEnv(env_);
    return false;
  }
  return true;
}

ONNXSession::~ONNXSession() {
  api_->ReleaseSession(session_);
  api_->ReleaseEnv(env_);
}

bool ONNXSession::runInference(const std::vector<float>& input_data,
                               const std::vector<int64_t>& dims,
                               std::vector<float>& output) {
  // 设置输入
  OrtValue* input_tensor;
  OrtMemoryInfo* mem_info = nullptr;
  OrtStatus* mem_status = api_->CreateCpuMemoryInfo(
      OrtDeviceAllocator, OrtMemTypeDefault, &mem_info);
  if (mem_status != nullptr) {
    LOG(WARNING) << "Failed to create memory info:"
                 << api_->GetErrorMessage(mem_status);
    if (mem_info != nullptr) {
      api_->ReleaseMemoryInfo(mem_info);
    }
    api_->ReleaseStatus(mem_status);
    return false;
  }
  OrtStatus* input_status = api_->CreateTensorWithDataAsOrtValue(
      mem_info, reinterpret_cast<void*>(const_cast<float*>(input_data.data())),
      input_data.size() * sizeof(float), dims.data(), dims.size(),
      ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &input_tensor);
  if (input_status != nullptr) {
    LOG(WARNING) << "Failed to create input tensor:"
                 << api_->GetErrorMessage(input_status);
    api_->ReleaseStatus(input_status);
    api_->ReleaseMemoryInfo(mem_info);
    return false;
  }
  OrtValue* output_tensor_;
  const char* input_names[1]={"input"};
  const char* output_names[1]={"output"};
  // 运行推理
  OrtStatus* run_status = api_->Run(session_, nullptr, input_names, &input_tensor,
                                    1, output_names, 1, &output_tensor_);
  if (run_status != nullptr) {
    LOG(WARNING) << "Failed to run inference:"
                 << api_->GetErrorMessage(run_status);
    api_->ReleaseStatus(run_status);
    api_->ReleaseMemoryInfo(mem_info);
    return false;
  }

  // 获取输出
  float* output_data;
  OrtStatus* output_status = api_->GetTensorMutableData(
      output_tensor_, reinterpret_cast<void**>(&output_data));
  if (output_status != nullptr) {
    LOG(WARNING) << "Failed to get output data:"
                 << api_->GetErrorMessage(output_status);
    api_->ReleaseStatus(output_status);
    api_->ReleaseMemoryInfo(mem_info);
    return false;
  }
  api_->ReleaseMemoryInfo(mem_info);
  output.assign(output_data, output_data + output_dims_size_);
  return true;
}
