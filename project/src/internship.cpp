#include <chrono>
#include <fstream>
#include <iostream>

#include <date/date.h>
#include <nlohmann/json.hpp>

#include "internship.h"

using json = nlohmann::json;
using namespace date;

struct OS {
    std::string name;
    std::string cycle;
    int days;
};

//this function is used to sort the vector of operating systems
bool compareOs(OS &os1, OS &os2) {
    if (os1.days>os2.days) {
        return true;
    } else {
        return false;
    }
}

namespace internship {

    std::vector<OS> mySolution(const std::string& jsonFileName, int elementsCount) {
        
        std::ifstream f(jsonFileName);
        json data = json::parse(f);

        std::vector<OS> allOperatingSystems;

        for (size_t i = 0; i < data.size(); i++) {

            if (data[i]["os"]==true) {

                for (size_t versionIterator = 0; versionIterator<data[i]["versions"].size(); versionIterator++) {

                    if (data[i]["versions"][versionIterator].contains("releaseDate") && data[i]["versions"][versionIterator].contains("eol") && data[i]["versions"][versionIterator]["releaseDate"].is_string() && data[i]["versions"][versionIterator]["eol"].is_string()) {

                        std::string date1 = data[i]["versions"][versionIterator]["releaseDate"];
                        std::string date2 = data[i]["versions"][versionIterator]["eol"];
                        
                        std::istringstream is1(date1);
                        std::istringstream is2(date2);
                        
                        date::sys_days d1, d2;
                        is1 >> date::parse("%F", d1);
                        is2 >> date::parse("%F", d2);

                        auto diff = d2 - d1;
                        int days = diff.count();

                        OS currentOs;
                        currentOs.name = data[i]["name"];
                        currentOs.cycle = data[i]["versions"][versionIterator]["cycle"];
                        currentOs.days = days;

                        allOperatingSystems.push_back(currentOs);
                    
                    } else {
                        //if the program reaches this bit, it means a version either has missing "releaseDate" or "eol",
                        //or the "eol" or "releaseDate" is not a string. In this case, we can just disregard that particular object.
                    }

                }

            }
        }

        std::sort(allOperatingSystems.begin(), allOperatingSystems.end(), compareOs);

        if (elementsCount>allOperatingSystems.size()) {
            std::cout << "WARNING! You are trying to get " << elementsCount << " operating systems, but there are only " << allOperatingSystems.size() << "\n";
            std::cout << "proceeding to print out all of the operating systems\n";
        }

        int howManyDoWePrint;

        if (elementsCount>allOperatingSystems.size()) {
            howManyDoWePrint = allOperatingSystems.size();
        } else {
            howManyDoWePrint = elementsCount;
        }

        for (int a=0; a<howManyDoWePrint; a++) {
            //we have to add one to the number of days, since we are including the day of the eol date as well
            std::cout << allOperatingSystems[a].name << " " << allOperatingSystems[a].cycle << " " << allOperatingSystems[a].days+1 << "\n";
        }

        //make sure that the vector we are returning for tests is the right size
        while (allOperatingSystems.size()>elementsCount) {
            allOperatingSystems.pop_back();
        }

        return allOperatingSystems;
    }

    // do not remove this function
    void solution(const std::string& jsonFileName, int elementsCount) {

        // put the call to your solution here
        mySolution(jsonFileName, elementsCount);

    }
}