#include "gtest/gtest.h"

#include "frontend/an2cn.h"

TEST(ConvertTest, T1) {
  AN2CN helper;


  double input1=1234506.78;
  std::string expected_output = "一百二十三万四千五百零六点七八";
  std::string actual_output = helper.convert2ChineseNumber(input1);
  EXPECT_EQ(expected_output, actual_output);

  double input2=1212300056.003;
  expected_output = "十二亿一千二百三十万零五十六点零零三";
  actual_output = helper.convert2ChineseNumber(input2);
  EXPECT_EQ(expected_output, actual_output);


  double input3=1200130014.;
  expected_output = "十二亿零十三万零十四";
  actual_output = helper.convert2ChineseNumber(input3);
  EXPECT_EQ(expected_output, actual_output);

}
