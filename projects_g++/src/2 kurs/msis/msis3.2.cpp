#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>

class str {
    public: 
    std::string abc;
    std::string H;
    std::string K;
    std::string rules; 
    int numB = 0;
    int numL = 0;
};

void inputFile(int answer[1000][4], int predAnswer[1000][4], str str[1000]) {
    std::fstream file("./src/smf/msis/tests2.txt");

    for (size_t i = 0; i < 1000; i++)
    {
        file << predAnswer[i][0];
        file << ' ';
        file << predAnswer[i][1];
        file << ' ';
        file << predAnswer[i][2];
        file << ' ';
        file << predAnswer[i][3];
        file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';
        file << answer[i][0];
        file << ' ';
        file << answer[i][1];
        file << ' ';
        file << answer[i][2];
        file << ' ';
        file << answer[i][3];
        file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';
        file << str[i].rules;
        file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';
        file << str[i].H;
        file << str[i].abc;
        file << str[i].K;
        file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';
        file << str[i].numL;
        file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';file << ' '; file << ' '; file << ' ';
        file << str[i].numB;
        file << '\n';
    }
    
}

int main() {
    std::vector<int> valueA;
    std::vector<int> valueB;
    std::vector<int> valueC;
    std::vector<int> valueD;
    int answer[1000][4];
    int predAnswer[1000][4];
    str str[1000];
    std::vector<int> valueX;
    std::vector<int> valueY;
    std::vector<int> valueZ;
    std::vector<int> valueR;

    for (size_t i = 0; i < 1000; i++)
    {
        valueA.push_back(abs(rand() % 10));
        valueB.push_back(abs(rand() % 8));
        valueC.push_back(abs(rand() % 12));
        valueD.push_back(abs(rand() % 10));
        str[i].abc = "";
        str[i].H = "H-";
        str[i].K = "K";
        str[i].rules = "";
        valueX.push_back(10);
        valueY.push_back(10);
        valueZ.push_back(10);
        valueR.push_back(10);
    }
    // int abc = rand() % 10 + 10;
    for (size_t i = 0; i < 1000; i++)
    {
        predAnswer[i][0] = valueA[i];
        predAnswer[i][1] = valueB[i];
        predAnswer[i][2] = valueC[i];
        predAnswer[i][3] = valueD[i];

        str[i].numB++;
        str[i].abc += "B1-";
        if (valueD[i] < 5)
        {
            str[i].abc += "Л1-";
            str[i].rules += "D<5,";
            str[i].numL++;
            str[i].abc += "B2-";
            valueR[i] += valueY[i];
            str[i].numB++;
        } else {
            str[i].abc += "Л1-";
            str[i].rules += "D<5,";
            str[i].numL++;
        }
        if (valueB[i] < 6)
        {
            str[i].abc += "Л2-";
            str[i].rules += "B<6,";
            str[i].numL++;
        } else {
            str[i].abc += "Л2-";
            str[i].rules += "B<6,";
            str[i].numL++;
            if (valueB[i] < 7)
            {
                str[i].abc += "Л3-";
                str[i].abc += "B3-";
                str[i].rules += "B<7,";
                str[i].numL++;
                valueY[i] *= 2;
                str[i].numB++;
            } else {
                str[i].abc += "Л3-";
                str[i].rules += "B<7,";
                str[i].numL++;
                if (valueB[i] == 8)
                {
                    str[i].abc += "Л4-";
                    str[i].rules += "B=8,";                    
                    str[i].numL++;                                        
                } else {
                    str[i].abc += "Л4-";
                    str[i].abc += "B4-";
                    str[i].rules += "B=8,";
                    valueY[i] += valueX[i];
                    str[i].numL++;
                    str[i].numB++;
                }                
            }
        }
        if (valueC[i] < 9)
        {
            str[i].abc += "Л5-";
            str[i].rules += "C<9,";
            str[i].numL++;
            if (valueC[i] > 10)
            {
                str[i].abc += "Л6-";
                str[i].rules += "C>10,";
                str[i].numL++;
                if (valueD[i] == 4)
                {
                    str[i].abc += "Л7-";
                    str[i].rules += "D=4,";
                    str[i].numL++;                    
                } else {
                    str[i].abc += "Л7-";
                    str[i].rules += "D=4,";
                    str[i].abc += "B6-";
                    str[i].numL++;
                    valueR[i] = 4;
                    str[i].numB++;
                }                
            } else {
                str[i].abc += "Л6-";
                str[i].rules += "C>10,";
                str[i].numL++;
            }            
        } else {
            str[i].abc += "Л5-";
            str[i].rules += "C<9,";
            str[i].abc += "B5-";
            str[i].numL++;
            valueZ[i] = valueX[i] + valueY[i];
            str[i].numB++;
        }
        answer[i][0] = valueX[i];
        answer[i][1] = valueY[i];
        answer[i][2] = valueZ[i];
        answer[i][3] = valueR[i];        
    }
    inputFile(answer, predAnswer, str);
}