
#include <iostream>
#include <vector>

#include "onnxruntime_c_api.h"

class ONNXSession {
public:
    explicit ONNXSession(int output_size);
    virtual ~ONNXSession();
    bool loadModel(const std::string& path);
    bool runInference(const std::vector<float>& input_data, const std::vector<int64_t>& dims,std::vector<float>& output);

private:
    static const OrtApiBase* gApiBase;
    const OrtApi* api_=nullptr;
    OrtEnv* env_=nullptr;
    OrtSession* session_=nullptr;
    int output_dims_size_ =0;

};
