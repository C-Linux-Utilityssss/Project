#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

vector<string> split(string sentence, char Separator)
{
    vector<string> answer;
    stringstream ss(sentence);
    string tmp;
 
    while (getline(ss, tmp, Separator)) {
        answer.push_back(tmp);
    }
    return answer;
}

void setShortcut(vector<string> command);
void runCommand(vector<string> command);

int main(){
    string field = "#";
    #if __linux__
        field = "$ ";
    #elif _WIN32
        field = ">";
    #endif

    fs::path currentDirectory = fs::current_path().root_path();
    fs::current_path(currentDirectory);
    string stringCommand;
    vector<string> command;

    while(true) {
        cout << currentDirectory.string() << field; 
        getline(cin, stringCommand);
        command = split(stringCommand, ' ');
        if(command[0] == "shortcut") {
            /* 이명재가 구현할 함수 */
            setShortcut(command);
        } else {
            /* 이상진이 구현할 함수 */
            runCommand(command);
        }
    }
    return 0;
}

void setShortcut(vector<string> command){

}
void runCommand(vector<string> command){

}