#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using UString = std::basic_string<uint16_t>;
class BertTokenizer{
 public:
  bool Init(std::string vocab);
  bool InitByFileContent(std::string content);
  std::vector<int> Encode(std::string text);
  int Word2Id(std::string word) const;
  std::string Id2Word(int id) const;
  int PadId() const;
  int MaskId() const;
  int SepId() const;
  int ClsId() const;
  int UnkId() const;
  int TotalSize() const;
 private:
  void max_seg_(std::string s, std::vector<int>& results);
  void load_vocab_(std::string path, std::vector<std::string>& lines);
  void init_from_lines(const std::vector<std::string>& lines);
  UString _basic_tokenize(UString text);
  UString _clean(UString text);
  std::unordered_map<std::string, int> token_2_id_map_;
  std::vector<std::string> tokens_;
  static std::string kUnkToken;
  static std::string kMaskToken;
  static std::string kSepToken;
  static std::string kPadToken;
  static std::string kClsToken;
};
