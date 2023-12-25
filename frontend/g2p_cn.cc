#include "frontend/g2p_cn.h"

#include <regex>
#include <tuple>
#include <algorithm>


#include "absl/log/check.h"
#include "absl/strings/str_join.h"
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

static std::string split_py(const std::string& py) {
    std::string tone = py.substr(py.length() - 1);
    std::string pinyin = py.substr(0, py.length() - 1);
    std::string sm = "";
    std::string ym = "";
    std::string suf_r = "";

    if (pinyin.front() =='n' || pinyin.front() =='m'){
        pinyin = std::string("e") + pinyin;
    }

    if (pinyin.back() == 'r') {
        suf_r = "r";
        pinyin.pop_back();
    }

    if (pinyin == "zi" || pinyin == "ci" || pinyin == "si" || pinyin == "ri") {
        sm = pinyin.substr(0, 1);
        ym = "ii";
    } else if (pinyin == "zhi" || pinyin == "chi" || pinyin == "shi") {
        sm = pinyin.substr(0, 2);
        ym = "iii";
    } else if (pinyin == "ya" || pinyin == "yan" || pinyin == "yang" || pinyin == "yao" || pinyin == "ye" || pinyin == "yong" || pinyin == "you") {
        sm = "";
        ym = 'i' + pinyin.substr(1);
    } else if (pinyin == "yi" || pinyin == "yin" || pinyin == "ying") {
        sm = "";
        ym = pinyin.substr(1);
    } else if (pinyin == "yu" || pinyin == "yv" || pinyin == "yuan" || pinyin == "yvan" || pinyin == "yue " || pinyin == "yve" || pinyin == "yun" || pinyin == "yvn") {
        sm = "";
        ym = 'v' + pinyin.substr(2);
    } else if (pinyin == "wu") {
        sm = "";
        ym = "u";
    } else if (pinyin[0] == 'w') {
        sm = "";
        ym = "u" + pinyin.substr(1);
    } else if (pinyin.length() >= 2 && (pinyin[0] == 'j' || pinyin[0] == 'q' || pinyin[0] == 'x') && pinyin[1] == 'u') {
        sm = pinyin[0];
        ym = 'v' + pinyin.substr(2);
    } else {
        auto seg_pos = pinyin.find_first_of("aeiou");
        sm = pinyin.substr(0, seg_pos);
        ym = pinyin.substr(seg_pos);

        if (ym == "ui") {
            ym = "uei";
        } else if (ym == "iu") {
            ym = "iou";
        } else if (ym == "un") {
            ym = "uen";
        } else if (ym == "ue") {
            ym = "ve";
        }
    }

    ym += suf_r + tone;
    return sm + " " + ym;
}
std::string G2PChinese::Process(const std::string &input) {
  std::vector<std::string> words;
  jieba_->Cut(input, words, true);
  std::string output;
  std::vector<std::string> results;
  results.emplace_back("<sos/eos>");
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
    if(std::any_of(pinyins.begin(), pinyins.end(), [&](const std::string& s) {
      return std::regex_search(s, std::regex("[\u3002\uff0c\uff1f\uff01\uff1b\uff1a\u201c\u201d\u2018\u2019\u300a\u300b\u3008\u3009\u3010\u3011\u300e\u300f\u2014\u2026\u3001\uff08\uff09]"));
    }) || std::any_of(pinyins.begin(), pinyins.end(), [&](const std::string& s) {
      return std::regex_search(s, std::regex("[!\"#$%&'()*+, -./:;<=>?@[\\]^_`{|}~]"));
    }) ){
      results.pop_back();
      results.emplace_back("sp3");
    }else{
      std::string str;
      for(auto pinyin : pinyins){
        std::string tmp = split_py(pinyin);
        if(!str.empty()){
          str.append(" sp0 ");
        }
        str.append(tmp);
      }
      results.emplace_back(str);
      results.emplace_back("sp1");
    }

  }
  results.emplace_back("<sos/eos>");
  return absl::StrJoin(results, " ");
}

