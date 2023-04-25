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
    return (os1.days > os2.days);
}

namespace internship {

    void mySolution(const std::string &jsonFileName, int elementsCount) {

        std::ifstream f(jsonFileName);
        json data = json::parse(f);

        std::vector <OS> allOperatingSystems;

        for (size_t i = 0; i < data.size(); i++) {

            if (data[i]["os"] == true) {

                for (size_t versionIterator = 0; versionIterator < data[i]["versions"].size(); versionIterator++) {

                    json currentVersion = data[i]["versions"][versionIterator];

                    if (currentVersion.contains("releaseDate") &&
                        currentVersion.contains("eol") &&
                        currentVersion["releaseDate"].is_string() &&
                        currentVersion["eol"].is_string()) {

                        //Now we have included in our search only the versions which are an OS,
                        //have "eol" and "releaseDate" fields which are both strings. 
                        //Any other objects which do not meet those requirements will not be included in the answer.
                        
                        std::string releaseDateString = currentVersion["releaseDate"];
                        std::string eolDateString = currentVersion["eol"];

                        std::istringstream is1(releaseDateString);
                        std::istringstream is2(eolDateString);

                        date::sys_days releaseDate, eolDate;
                        is1 >> date::parse("%F", releaseDate);
                        is2 >> date::parse("%F", eolDate);

                        auto diff = eolDate - releaseDate;
                        int days = diff.count();

                        OS currentOs;
                        currentOs.name = data[i]["name"];
                        currentOs.cycle = currentVersion["cycle"];
                        currentOs.days = days;

                        allOperatingSystems.push_back(currentOs);

                    }

                }
            }
        }

        std::sort(allOperatingSystems.begin(), allOperatingSystems.end(), compareOs);

        // I have decided to add this, because in the task description it was not specified what to do when for example we want to
        // print info about a 100 operating systems, but there are only 5. Since in that case we can only print as many as there are,
        // I have decided to add this warning so that the user is notified in this edge case
        if (elementsCount > allOperatingSystems.size()) {
            std::cout << "WARNING! You are trying to get " << elementsCount << " operating systems, but there are only "
                      << allOperatingSystems.size() << "\n";
            std::cout << "proceeding to print out all of the operating systems\n";
        }

        int howManyDoWePrint = (elementsCount > allOperatingSystems.size()) ? allOperatingSystems.size() : elementsCount;

        if (elementsCount > allOperatingSystems.size()) {
            howManyDoWePrint = allOperatingSystems.size();
        } else {
            howManyDoWePrint = elementsCount;
        }

        for (int i = 0; i < howManyDoWePrint; i++) {
            //we have to add one to the number of days, since we are including the day of the eol date as well
            std::cout << allOperatingSystems[i].name << " " << allOperatingSystems[i].cycle << " "
                      << allOperatingSystems[i].days + 1 << "\n";
        }
    }

    // do not remove this function
    void solution(const std::string &jsonFileName, int elementsCount) {

        // put the call to your solution here
        mySolution(jsonFileName, elementsCount);
    }
}