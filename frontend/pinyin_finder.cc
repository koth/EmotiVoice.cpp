#include "pinyin_finder.h"

#include <regex>
#include <vector>
#include <locale>
#include <climits>
#include <cwchar>

#include "absl/log/check.h"
#include "absl/log/log.h"

PinyinFinder::PinyinFinder() {}

PinyinFinder::~PinyinFinder() {}


bool PinyinFinder::init(const std::string& singleCharacterDictPath,
          const std::string& wordsDictPath) {
  std::regex pattern(
      "U[+]([0-9A-F]{4,5}):\\s+([a-zāáǎàēéěèīíǐìōóǒòūúǔùüǘǚǜńňḿm̀,]+)\\s+#\\s+(\\S+)\\s*");
  
  FILE* fp = fopen(singleCharacterDictPath.c_str(), "r");
  if (fp == NULL) {
    LOG(WARNING) << "Failed to open file: " << singleCharacterDictPath;
    return false;
  }
  char line[1024] = {0};
  int tn =0;
  while (fgets(line, sizeof(line) - 1, fp)) {
    if(strlen(line)>1 && line[0]=='#'){
      continue;
    }
    std::string str(line, strlen(line));
    std::smatch match;
    CHECK(regex_match(str, match, pattern)) << "match error for line: " << str;
    uint32_t unicode = stoul(match[1], nullptr, 16);
    
    std::string  pinyin = match[2];
    UnicodeStr ustr;
    ustr.append(1, static_cast<UnicodeCharT>(unicode));
    std::string tstr;
    BasicStringUtil::u16tou8(ustr.data(), ustr.size(), tstr);
    
    std::vector<std::string> ss;
    BasicStringUtil::SplitString(pinyin.c_str(), pinyin.size(), ',', &ss);
    word_pinyin_dict_[ustr] = ss[0];
    tn +=1;
    
    // Process each line here...
  }
  LOG(INFO)<<"total pinyin character count: "<<tn;

  fclose(fp);
  fp = fopen(wordsDictPath.c_str(), "r");
  if (fp == NULL) {
    LOG(WARNING) << "Failed to open file: " << wordsDictPath;
    return false;
  }
  std::regex pattern2(
      "(\\S+):\\s+([a-zāáǎàēéěèīíǐìōóǒòūúǔùüǘǚǜńňḿm̀ ]+)\\s*");
  int pc =0;
  while (fgets(line, sizeof(line) - 1, fp)) {
    if(strlen(line)>1 && line[0]=='#'){
      continue;
    }
    int end = strlen(line);
    for(int i=0;i<end;i++){
      if(line[i]=='#'){
        end = i;
        break;
      }
    }
    std::string str(line, end);
    std::smatch match;
    CHECK(regex_match(str, match, pattern2)) << "match error for line: " << str;
    std::string word = match[1];
    std::string pinyin = match[2];
    // LOG(INFO)<<"got word: "<<word<<" pinyin: "<<pinyin;
    UnicodeStr ustr;
    BasicStringUtil::u8tou16(word.c_str(), word.size(), ustr);
    word_pinyin_dict_[ustr] = pinyin;

    pc +=1;

  }
  LOG(INFO)<<"total pinyin phrase count: "<<pc;
  return true;
}

void PinyinFinder::find_best_pinyin(const std::string& phrasestr,std::vector<std::string>& pinyins) {
  UnicodeStr phrase;
  BasicStringUtil::u8tou16(phrasestr.c_str(), phrasestr.size(), phrase);
  int n = phrase.size();
  std::vector<std::vector<int>> dp(n, std::vector<int>(n, std::numeric_limits<int>::max()));
  std::vector<std::vector<int>> opts(n, std::vector<int>(n, -1));
  for (int length = 1; length <= n; ++length) {
    for (int i = 0; i <= n - length; ++i) {
      int j = i + length - 1;
      if (length == 1) {
        dp[i][j] = 1;
        opts[i][j] = j;
      } else {
        int maxtry = length;
        if (length > kMaxChars) {
          maxtry = kMaxChars;
        }
        for (int k = maxtry; k >= 1; k--) {
          int to = i + k - 1;
          UnicodeStr sub = phrase.substr(i, k);
          // if(i==0 && j == 6){
          //   std::string tstr;
          //   BasicStringUtil::u16tou8(sub.data(), sub.size(), tstr);
          //   LOG(INFO)<<"sub: "<<tstr<<",k:"<<k;
          // }
          if (word_pinyin_dict_.find(sub) != word_pinyin_dict_.end()) {
            if (to == j) {
              dp[i][j] = k == 1 ? 1 : 0;
              opts[i][j] = j;
            } else {
              if (k == 1) {
                if (dp[i][j] > dp[to + 1][j] + 1) {
                  dp[i][j] = dp[to + 1][j] + 1;
                  opts[i][j] = to;
                }
              } else {
                if (dp[i][j] > dp[to + 1][j]) {
                  dp[i][j] = dp[to + 1][j];
                  opts[i][j] = to;
                }
              }
            }
          }
        }
      }
    }
  }

  // Construct the best pinyin string using dp and opts
  std::string best_pinyin;
  int i = 0;
  int j = n - 1;
  while (i <= j) {
    int opt = opts[i][j];
    std::string pinyin;
    if(opt ==-1){
      opt =i;
    }
    UnicodeStr sub = phrase.substr(i, opt - i + 1);
    auto it = word_pinyin_dict_.find(sub);
    std::string tstr;
    BasicStringUtil::u16tou8(sub.data(), sub.size(), tstr);
    if(it == word_pinyin_dict_.end()){
      pinyins.emplace_back(tstr);
      // LOG(WARNING) << "sub not in dict,i:"<<i<<" j:"<<j<<" opt:"<<opt<<" sub: "<<tstr;
    }else{
      std::vector<std::string> tmps;
      BasicStringUtil::SplitString(it->second.c_str(), it->second.size(),' ', &tmps);
      for(auto str: tmps){
        pinyins.emplace_back(str);
      }
    }
    i = opt + 1;
  }

}

