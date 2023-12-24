#include "an2cn.h"


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

std::map<char, std::string> chinese_numerals = {
    {'0', "零"},
    {'1', "一"},
    {'2', "二"},
    {'3', "三"},
    {'4', "四"},
    {'5', "五"},
    {'6', "六"},
    {'7', "七"},
    {'8', "八"},
    {'9', "九"}
};

std::string convert_group(const std::string& num_str) {
    std::string result;
    int zero_count = 0;
    std::vector<std::string> units = {"", "十", "百", "千"};

    for (size_t i = 0; i < num_str.size(); ++i) {
        char digit = num_str[i];
        if (digit == '0') {
            zero_count++;
        } else {
            if (zero_count > 0) {
                result += chinese_numerals['0'];
                zero_count = 0;
            }
            if((result.empty()  || result.substr(result.size()-3)=="零") && digit =='1' && (num_str.size() - 1 - i)==1){
                result += units[num_str.size() - 1 - i];
            }else{
                result += chinese_numerals[digit] + units[num_str.size() - 1 - i];
            }
        }
    }
    return result;
}

std::string convert_to_chinese_numerals(long long number) {
    if (number == 0) {
        return "零";
    }
    std::vector<std::string> big_units = {"", "万", "亿", "兆"};
    std::string str_num = std::to_string(number);
    std::reverse(str_num.begin(), str_num.end());

    std::string chinese_str;
    for (size_t i = 0; i < str_num.size(); i += 4) {
        std::string group = str_num.substr(i, 4);
        std::reverse(group.begin(), group.end());
        std::string chinese_part = convert_group(group);
        if (!chinese_part.empty()) {
            chinese_str = chinese_part + big_units[i / 4] + chinese_str;
        }
    }

    return chinese_str;
}

// 将个位数字转换为中文数字
std::string digitToChinese(int digit) {
    const std::vector<std::string> chineseDigits = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九"};
    return chineseDigits[digit];
}

// 转换小数部分
std::string convertDecimalPart(double number) {
    std::string chineseDecimal = "";
    // epsilon set to 1e-4
    while (number > 1e-4) {
        number *= 10;
        int digit = static_cast<int>(number) % 10;
        chineseDecimal += digitToChinese(digit);
        number -= digit;
    }
    return chineseDecimal;
}

std::string AN2CN::convert2ChineseNumber(double number){

    long long int_part = static_cast<long long>(number);
    double decimal_part = number - int_part;
    std::string chineseNumber = convert_to_chinese_numerals(int_part);
    if (decimal_part > 0) {
        // 处理小数部分
        chineseNumber += "点" + convertDecimalPart(decimal_part);
    }
    return chineseNumber;

}
