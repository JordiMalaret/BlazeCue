#include <iostream>
#include <array>
#include "heartRate.h"

const int16_t ARRAY_SIZE = 1024;

std::array<int16_t, ARRAY_SIZE> split(){
    std::array<int16_t, ARRAY_SIZE> dataArr;
    int count = 0;
    while(count<1024){
        int16_t data = getCurrentACVal();
        dataArr[count] = data;
        count++;
    }
    return(dataArr);
}
