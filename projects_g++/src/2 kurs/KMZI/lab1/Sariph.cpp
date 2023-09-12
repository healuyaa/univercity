#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <sstream>

class ariph 
{
    public:

    char Symbol;
    double Left;
    double Right;
};

std::vector<ariph> DefineSeg(std::string letters, std::vector<double> probabilities) 
{
    int m = letters.length();
    std::vector<ariph> ariph(m);
    double l = 0;

    for (int i = 0; i < m; i++) {
        ariph[i].Left = l;
        ariph[i].Right = l + probabilities[i];
        ariph[i].Symbol = letters[i];
        l = ariph[i].Right;
    }
    return ariph;
}

double ArithmeticEncoding(std::string letters, std::vector<double> probabilities, std::string str) 
{
    auto ariph = DefineSeg(letters, probabilities);
    double left = 0, right = 1;

    for (int i = 0; i < str.length(); i++) {
        int symb = find(letters.begin(), letters.end(), str[i]) - letters.begin();
        double newRight = left + (right - left) * ariph[symb].Right;
        double newLeft = left + (right - left) * ariph[symb].Left;
        left = newLeft;
        right = newRight;
    }
    return (left + right) / 2;
}

std::string ArithmeticDecoding(std::string letters, std::vector<double> probabilities, double key, int n) 
{
    auto seg = DefineSeg(letters, probabilities);
    std::string str = "";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < letters.length(); j++) {
            if (key >= seg[j].Left && key < seg[j].Right) {
                str += seg[j].Symbol;
                key = (key - seg[j].Left) / (seg[j].Right - seg[j].Left);
                break;
            }
        }
    }
    return str;
}

std::vector<double> FindProbability(std::string str, std::string letters) 
{
    std::vector<double> probabilities;
    for (char c : letters) {
        auto match = std::count(str.begin(), str.end(), c);
        probabilities.push_back((double)match / str.length());
    }
    return probabilities;
}

int main()
{
    std::string to_compress = "first";
    std::string letters = "abcdefghijklmnopqrstuvwxyz! ,.";
    std::cout << "data: " << to_compress << std::endl;
    
    auto probability = FindProbability(to_compress, letters);
    double to_uncompress = ArithmeticEncoding(letters, probability, to_compress);
    std::cout << "to_compress: " << to_uncompress << std::endl;

    std::string from_uncompress = ArithmeticDecoding(letters, probability, to_uncompress, to_compress.length());
    std::cout << "from_uncompress: " << from_uncompress << std::endl;
}