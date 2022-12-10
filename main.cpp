#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

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

void setShortcut(vector<string> command, fs::path& currentDirectory);
void runCommand(vector<string> command, fs::path& currentDirectory);

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
        if(command[0] == "shortcut") setShortcut(command, currentDirectory);
        else runCommand(command, currentDirectory);
    }
    return 0;
}

void setShortcut(vector<string> command, fs::path& currentDirectory){
    /* 인자가 3개인 경우 .shct 파일을 만듦
    인자는 순서대로 (shortcut <.shct 파일 이름> <경로>) */
    if(command.size() == 3) {
        if(!fs::exists(command[2])) { // 파일이 존재하지 않을 때
            cout << "No such file or directory\n";
            return;
        }
        if(fs::exists(command[1] + ".shct")) {
            cout << command[1] + ".shct file is already existent\n";
            return;
        }
        ofstream shctFile(command[1] + ".shct"); // 파일 생성 및 쓰기
        fs::path absolutePath = command[2]; // 2번째 인자 path 객체 형태로 가져오기
        shctFile << fs::canonical(absolutePath).string(); // 절대경로로 나타내기
        shctFile.flush();
        shctFile.close();
    // 인자가 2개인 경우 .shct 파일에 적힌 경로로 이동
    } else if(command.size() == 2) {
        ifstream shctFile(command[1] + ".shct");
        if(!shctFile) {
            cout << "No such file or directory\n";
            return;
        }
        string pathToChange;
        getline(shctFile, pathToChange);
        fs::current_path(pathToChange);
        currentDirectory = fs::current_path();
    // 인자의 개수가 맞지 않으면 에러메세지를 내보냄
    } else {
        cout << "The number of arguments does not match\n";
    }
}
void runCommand(vector<string> command, fs::path& currentDirectory){

    currentDirectory = fs::current_path();
}