#include "frontend/g2p_cn.h"

#include <regex>
#include <tuple>

#include "absl/log/check.h"
#include "frontend/pinyin_finder.h"
#include "cppjieba/Jieba.hpp"


const std::string DICT_PATH = "dict/jieba.dict.utf8";
const std::string HMM_PATH = "dict/hmm_model.utf8";
const std::string USER_DICT_PATH = "dict/user.dict.utf8";
const std::string IDF_PATH = "dict/idf.utf8";
const std::string STOP_WORD_PATH = "dict/stop_words.utf8";

void split_string_numeric_other(const std::string& s, std::vector<std::string>& result) {
  std::regex pattern(R"(\d[\d\.]*)");
  std::smatch matches;
  std::string numeric_part;
  std::string other_part;
  std::string cur =s;
  // Search for all occurrences of the pattern in the input string
  while (std::regex_search(cur, matches, pattern)) {
    result.push_back(cur.substr(0, matches.position()));
    result.push_back(matches[0]);
    // Update the input string to search in the remaining part
    cur = cur.substr(matches.position() + matches[0].length());
  }
  if(!cur.empty()){
    result.push_back(cur);
  }
}


G2PChinese::G2PChinese() {
  jieba_.reset(new cppjieba::Jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH,
                                   IDF_PATH, STOP_WORD_PATH));
  pinyin_finder_.reset(new PinyinFinder());
  CHECK(pinyin_finder_->init("dict/pinyin.txt", "dict/large_pinyin.txt"));
}
G2PChinese::~G2PChinese() {}
std::string G2PChinese::Process(const std::string &input) {
  std::vector<std::string> words;
  jieba_->Cut(input, words, true);
  std::string output;
  for (const auto &word : words) {
    if(word.empty() || word ==" "){
        continue;
    }
    std::vector<std::string> parts;
    split_string_numeric_other(word, parts);
    std::string todo;
    for(const auto &part : parts){
        if(isdigit(part[0])){
            todo += " " + part;
        }else{
            todo += part;
        }
    }
    std::vector<std::string> pinyins;
    pinyin_finder_->find_best_pinyin(todo,pinyins);
    

  }
  return output;
}
