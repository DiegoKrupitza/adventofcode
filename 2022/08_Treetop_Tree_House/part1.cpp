#include <iostream>
#include <vector>
#include <string>
#include <utility>

struct HeightMax
{
    int topDown = 0;
    int bottomUp = 0;
    int leftToRight = 0;
    int rightToLeft = 0;
    int value = 0;

    HeightMax(int topDown = 0,int bottomUp = 0,int leftToRight = 0,int rightToLeft = 0,int value = 0) : topDown(topDown), bottomUp(bottomUp), leftToRight(leftToRight), rightToLeft(rightToLeft) , value(value)
    {
    }

};

std::ostream &operator<<(std::ostream &os, const HeightMax hm) {
    return os << "{" << "TD=" << hm.topDown << ",BU=" << hm.bottomUp << ",LR=" << hm.leftToRight << ",RL=" << hm.rightToLeft << ",V=" << hm.value << "}";
}


std::vector<std::vector<HeightMax>> calculateMaxes(const std::vector<std::vector<int>> &treeMap, const std::vector<std::vector<int>>::size_type ROWS, const std::vector<std::vector<int>>::size_type COLS)
{
    std::vector<std::vector<HeightMax>> maxes;

    // rows from 0 to rows-1
    // left to right 

    for(std::vector<std::vector<int>>::size_type i = 0; i < ROWS;i++)
    {
        std::vector<HeightMax> internalRow;
        internalRow.push_back({0,0,0,0,treeMap[i][0]});
        for(std::vector<std::vector<int>>::size_type j = 1; j < COLS;j++)
        {
            const HeightMax entry{0,0,std::max(internalRow[j-1].leftToRight,treeMap[i][j-1]),0,treeMap[i][j]};
            internalRow.push_back(entry);            
        }
        maxes.push_back(internalRow);
    }

    // right to left
    for(std::vector<std::vector<int>>::size_type i = 0; i < ROWS;i++)
    {
        for(std::vector<std::vector<int>>::size_type j = COLS-2; j != 0;j--)
        {
            maxes[i][j].rightToLeft = std::max(maxes[i][j+1].rightToLeft,treeMap[i][j+1]);
        }
        maxes[i][0].rightToLeft = std::max(maxes[i][1].rightToLeft,treeMap[i][1]);
    }

    // topDown & bottomUp
    for(std::vector<std::vector<int>>::size_type i = 0; i < COLS;i++)
    {
        for(std::vector<std::vector<int>>::size_type j = 1; j < ROWS;j++)
        {
            maxes[j][i].topDown = std::max(maxes[j-1][i].topDown,treeMap[j-1][i]);
            maxes[ROWS-1-j][i].bottomUp = std::max(maxes[ROWS-j][i].bottomUp,treeMap[ROWS-j][i]);
        }
    }

    return maxes;
}

int countNumberOfVisibleTrees(const std::vector<std::vector<int>> &treeMap, const std::vector<std::vector<int>>::size_type ROWS, const std::vector<std::vector<int>>::size_type COLS) 
{
    // the other ring is always visible by definition
    auto result = (ROWS*COLS) - ((ROWS-2)*(COLS-2));

    const auto maxes = calculateMaxes(treeMap,ROWS,COLS);

    for(std::vector<std::vector<int>>::size_type i = 1; i < ROWS-1;i++) 
    {
        for(std::vector<std::vector<int>>::size_type j = 1; j < COLS-1;j++)
        {
            const auto &col_e = maxes[i][j];
            //std::cout << col_e << ' ';
            result += (std::min({col_e.leftToRight,col_e.rightToLeft,col_e.bottomUp,col_e.topDown}) < col_e.value);
        }
        //std::cout << '\n';
    }

    return result;
}

int main() 
{
    std::vector<std::vector<int>> treeMap;

    for(std::string line;std::getline(std::cin,line);)
    {
        std::vector<int> internalRow;
        for(std::string::size_type j=0;j<line.length();j++) 
        {
            internalRow.push_back(line[j] - '0'); // neat little trick to convert number stored as char to int. e.g '3' = 51 , '0' = 48 ... '3' - '0' = 3
        }
        treeMap.push_back(internalRow);
    }

    auto numberOfVisibleTrees = countNumberOfVisibleTrees(treeMap,treeMap.size(),treeMap[0].size());

    std::cout << "Number of visible trees: " << numberOfVisibleTrees << std::endl;
}