#include "gtest/gtest.h"

#include "frontend/pinyin_finder.h"

TEST(PinyinFinderTest, T1) {
  PinyinFinder finder;
  EXPECT_TRUE(finder.init("dict/pinyin.txt", "dict/large_pinyin.txt"));
  std::string testLine="洪水退去，自救成了他最重要的事情。";
  std::vector<std::string> pinyins;
  finder.find_best_pinyin(testLine,pinyins);
  for(auto py: pinyins){
    std::cout <<"pinyin: "<<py<<std::endl;
  }
  
}
