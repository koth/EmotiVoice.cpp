#include <string>
#include <unordered_map>
#include <vector>

#include "utils/basic_string_util.h"

const static int kMaxChars = 6;

class PinyinFinder {
 public:
  PinyinFinder();
  virtual ~PinyinFinder();

  bool init(const std::string& singleCharacterDictPath,
            const std::string& wordsDictPath);

  void find_best_pinyin(const std::string& word,std::vector<std::string>& pinyins);

 private:
  std::unordered_map<UnicodeStr, std::string> word_pinyin_dict_;
};
