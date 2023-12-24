#pragma once

#include <memory>
#include <string>
#include <vector>

namespace cppjieba {
class Jieba;
}  // namespace cppjieba

class PinyinFinder;
class G2PChinese {
 public:
  G2PChinese();
  virtual ~G2PChinese();
  std::string Process(const std::string &input);

 private:
  std::unique_ptr<cppjieba::Jieba> jieba_;
  std::unique_ptr<PinyinFinder> pinyin_finder_;
};
