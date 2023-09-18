#include <iostream>
#include <iostream>
#include <vector>
#include <math.h>

class Quest1 {
    public:
    void show() {
        for(size_t i{}; i < month.size(); ++i) {
            values.push_back((float) (rand() % 1000) / 10);
        }
        for(size_t i{}; i < month.size(); ++i) {
            std::cout << i + 1 << "      " << month[i] << "     " << values[i] << std::endl;
        }
    }

    private:
    std::vector<std::string> month = { "january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
    std::vector<float> values;
};

class Quest2 { 
    public:
    void substr() {
        for (size_t i = 0; i < dates.size(); i++)
        {
            std::string date = dates[i];
            int day = std::stoi(date.substr(0, 2));
            int month = std::stoi(date.substr(3, 2));
            int year = std::stoi(date.substr(6, 4));
            
            std::cout << day << ' ' << months[month - 1] << ' ' << year << std::endl;
        }
    }

    private:
    std::vector<std::string> dates = {"01/09/2022", "15/03/2023", "20/07/2022", "05/12/2023", "10/05/2022", "25/11/2023", "07/02/2022", "18/08/2023", "30/06/2022", "12/01/2023"};
    std::vector<std::string> months = { "january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
};

class Quest3 {
    public: 
    std::string randStr() {
        int size_alph = alphabyte.size();
        int r_a = 0;
        std::string answer = "";

        for (size_t i{}; i < size_alph; ++i)
        {
            r_a = rand() % size_alph;
            answer += alphabyte[r_a];
        }
        return answer;        
    }

    void show() {
        std::string str = randStr();
        std::cout << str;
    }

    private:
    std::string alphabyte = "qwrtpsdfghjklzxcvbnm";
};

class Quest4 {
    public:
    void find_symbols() {
        int value_space = 0;
        int valu_letter_gl = 0;
        int size = str.size();
        for (size_t i{}; i < size; ++i)
        {
            if (str[i] == 'a' || str[i] == 'e' || str[i] == 'u' || str[i] == 'y' || str[i] == 'i' || str[i] == 'o')
            {
                ++valu_letter_gl;
            } else if (str[i] == ' ') {
                ++value_space;
            }
        }
        std::cout << "space: " << value_space << " letter gl: " << valu_letter_gl << " size: " << size << std::endl;
    }
    private:
    std::string str = "But eyes are blind. You have to look with the heart";
};

class Quest5 {
    public: 
    void str_cut(char c) {
        std::string temp;
        for(auto s : str) {
            if(s == c) {
                temp += c;
                std::cout << temp << std::endl;
                temp = "";
            } else {
                temp += s;
            }
        }        
    }
    private:
    std::string str = "Everywhere I go I find a poet has been there before me.";
};

int main() {
    // Quest1 q1;
    // q1.show();

    // Quest2 q2;
    // q2.substr();

    // Quest3 q3;
    // q3.show();

    // Quest4 q4;
    // q4.find_symbols();

    // Quest5 q5;
    // q5.str_cut('e');    
}