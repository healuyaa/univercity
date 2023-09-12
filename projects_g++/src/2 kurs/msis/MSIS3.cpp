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
    std::fstream file("./src/smf/msis/tests.txt");

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
        valueA.push_back(abs(rand() % 7));
        valueB.push_back(abs(rand() % 15));
        valueC.push_back(abs(rand() % 7));
        valueD.push_back(abs(rand() % 10));
        str[i].abc = "";
        str[i].H = "H-";
        str[i].K = "K";
        str[i].rules = "";
        valueX.push_back(23);
        valueY.push_back(23);
        valueZ.push_back(23);
        valueR.push_back(23);
    }
    // int abc = rand() % 10 + 10;
    for (size_t i = 0; i < 1000; i++)
    {
        predAnswer[i][0] = valueA[i];
        predAnswer[i][1] = valueB[i];
        predAnswer[i][2] = valueC[i];
        predAnswer[i][3] = valueD[i];

        if (valueA[i] < 3)  
        {
            str[i].numL++;
            str[i].rules += "A<3,";
            str[i].abc += "Л1-"; 
            if (valueC[i] < 5)
            {
                str[i].numL++;
                str[i].rules += "C<5,";
                str[i].abc += "Л3-"; 
            }else {
                str[i].numL++;
                str[i].rules += "C<5,";
                str[i].abc += "Л3-";
                valueX[i] = valueY[i] - 1;
                str[i].numB++;
                str[i].abc += "В3-";
            }
            
        } else {
            str[i].numL++;
            str[i].rules += "A<3,";
            str[i].abc += "Л1-";
            if(valueC[i] == 4) {
                str[i].numL++;
                str[i].rules += "C=4,";
                str[i].abc += "Л2-"; 
            } else {
                str[i].numL++;
                str[i].rules += "C=4,";
                str[i].abc += "Л2-";
                str[i].abc += "В2-"; 
                valueY[i] = 4;
                str[i].numB++;
            }
        }
        if(valueD[i] > 6) {
            str[i].numL++;
            str[i].rules += "D>6,";
            valueZ[i] = valueR[i] * valueX[i];
            str[i].numB++;
            str[i].abc += "Л4-";
            str[i].abc += "В4-";
        } else {
            str[i].numL++;
            str[i].rules += "D>6,";
            str[i].abc += "Л4-";
        }
        if(valueB[i] == 10) {
            str[i].numL++;
            str[i].rules += "B=10,";
            str[i].abc += "Л5-";
            if (valueD[i] < 4)
            {
                str[i].numL++;
                str[i].rules += "D<4,";
                str[i].abc += "Л6-";
                if (valueB[i] > 11)
                {
                    str[i].numL++;
                    str[i].rules += "B>11,";
                    str[i].abc += "Л7-";
                } else {
                    str[i].numL++;
                    str[i].rules += "B>11,";
                    str[i].abc += "Л7-";
                    valueR[i] = valueY[i] + valueZ[i];
                    str[i].numB++;
                    str[i].abc += "В5-";
                }                
            } else {
                str[i].numL++;
                str[i].rules += "D<4,";
                str[i].abc += "Л6-";
            }            
        } else {
            str[i].numL++;
            str[i].rules += "B=10,";
            str[i].abc += "Л5-";
        }

        answer[i][0] = valueX[i];
        answer[i][1] = valueY[i];
        answer[i][2] = valueZ[i];
        answer[i][3] = valueR[i];
        // std::cout << "   " << "A: " << answer[i][0] << "   " << "B: " << answer[i][1] << "   " << "C: " << answer[i][2] << "   " << "D: " << answer[i][3] << "      " <<
        // str[i].abc << "       " << "start values: " << predAnswer[i][0] << ' ' << predAnswer[i][1] << ' ' << predAnswer[i][2] << ' ' << predAnswer[i][3] << std::endl;
    } 
    inputFile(answer, predAnswer, str);
}



// Для Вантуса послание, всё что выглядит так --- str[i].rules += "B>11,"; --- нечто подобное, так надо!!!!!!!!!!!
// Это сохраняется последовательность простой знай это, тебе нужно передалать ветвления(ромбик в блок схеме), которые в 4 лабе по МСИСУ
// predAnswer - начальные значения, answer - итоговые значения, vectorA/B/C/D - рандом значений для входных, 
// class с названием str и в мейне имя str, так надо :D
// В class: abc - последовательность пройденных ромбиков и прямоугольничков
//          H и K - это буквы в начале и конце
//          rules - условия ветвлений
//          numB и numL - это для подсчёта количество вычилительных(прямоугольничек) и логических блоков(ромбик)
// ГЛАВНОЕ ЗАПОМНИТЬ ЕСЛИ ДА В РОМБИКЕ - НАПРАВО, НЕТ - НАЛЕВО