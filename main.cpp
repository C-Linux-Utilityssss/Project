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

string os = "linux";
int main(){
    string field = "#";
    #if __linux__
        field = "$ ";
        os = "linux";
    #elif _WIN32
        field = ">";
        os = "windows";
    #endif

    fs::path currentDirectory = fs::current_path().root_path();
    fs::current_path(currentDirectory);
    string stringCommand;
    vector<string> command;

    while(true) {
        cout << currentDirectory.string() << field; 
        getline(cin, stringCommand);
        if(!stringCommand.length()) {
            continue;
        }
        command = split(stringCommand, ' ');
        if(command[0] == "shortcut") setShortcut(command, currentDirectory);
        else runCommand(command, currentDirectory);
    }
    return 0;
}



/* Utilities */
bool isNotMatch(int size, int num) { 
    if(size != num)
        cout << "The number of arguments does not match\n";
    return size != num;
}

std::string getRightPaddingString(std::string const &str, int n, char paddedChar = ' ')
{
    std::ostringstream ss;
    ss << std::left << std::setfill(paddedChar) << std::setw(n) << str;
    return ss.str();
}

void findFileAsName(string path, string name) {
    for (const auto& entry : fs::directory_iterator(path)) {
        if(fs::is_directory(entry.path())) // 재귀적으로 특정 문자열을 가진 파일을 탐색함
            findFileAsName(entry.path(), name);
        else if(entry.path().string().find(name) != string::npos) {
            cout << entry.path().string() << endl;
        }
    }
}

int q(int L, int R, vector<fs::path>& list) {
    int low = L, high = R+1;
    string pivot = list[L].string();
    do {
        do {
            low++;
        } while(low < R + 1 && pivot.compare(list[low].string()) > 0);
        do {
            high--;
        } while(pivot.compare(list[high].string()) < 0);
        if(low < high) {
            string temp = list[low];
            list[low] = list[high];
            list[high] = temp;
        }
    } while(low < high);
    string temp = list[L];
    list[L] = list[high];
    list[high] = temp;
    return high;
}

void qSort(int L, int R, vector<fs::path>& list) {
    if(L < R) {
        int m = q(L, R, list);
        qSort(L, m-1, list);
        qSort(m+1, R, list);
    }
}



/*---------------------------------------------------------------------------------------------------*/



void setShortcut(vector<string> command, fs::path& currentDirectory){
    if(isNotMatch(command.size(), 2) || isNotMatch(command.size(), 3)) return;
    /* 인자가 3개인 경우 .shct 파일을 만듦
    인자는 순서대로 (shortcut <.shct 파일 이름> <경로>) */
    if(command.size() == 3) {
        if(!fs::exists(command[2])) { // 폴더가 존재하지 않을 때
            cout << "No such file or directory\n";
            return;
        }
        if(fs::exists(command[1] + ".shct")) { // .shct 파일이 존재할 때
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
    }
}
const int MAX = 100;
typedef struct Stack { // cd를 통해 경로를 이동했을 때 그 경로가 스택에 쌓임
    int top;
    string stack[MAX];
} Stack;



/*---------------------------------------------------------------------------------------------------*/



void runCommand(vector<string> command, fs::path& currentDirectory){
    error_code err; // 에러 확인용

    if(command[0] == "ls") { // ls
        if(isNotMatch(command.size(), 1)) return;
        int cnt = 0;
        int brCnt = 3; // 파일이나 디렉토리가 3번 출력될 때마다 줄바꿈을 함
        int max = 0;
        int numberOfList = 0;
        vector<fs::path> list;
        for (const auto& entry : fs::directory_iterator(currentDirectory)) {
            list.push_back(entry.path().string());
            numberOfList++;
            if(max < entry.path().string().length()) max = entry.path().string().length();
        }
        qSort(0, numberOfList-1, list);
        brCnt -= max / 60; // 파일이나 디렉토리의 최대 글자 수가 60자씩 넘을 때마다 줄바꿈을 하는 횟수를 증가시킴
        brCnt = brCnt <= 0 ? 1 : brCnt; // 만약 brCnt의 값이 0보다 작거나 같을 경우 1로 맞춰줌
        for (const auto& entry : list) {
            if(fs::is_directory(entry)) {
                cout << getRightPaddingString(entry.string().substr(entry.string().find_last_of("/")), max, ' ') + '\t';
            } else {
                cout << getRightPaddingString(entry.string().substr(entry.string().find_last_of("/") + 1), max, ' ') + '\t';
            }
            cnt++;
            if(cnt % brCnt == 0) { // 파일이나 디렉토리가 brCnt번 출력될 때마다 줄바꿈을 함
                cout << endl;
                cnt = 0;
            }
        }
        if(cnt) cout << endl;
    }

    else if(command[0] == "mkdir") { // mkdir
        if(isNotMatch(command.size(), 2)) return;
        if(!fs::create_directories(command[1], err)) {
            cout << "Failed to create directory, error: " 
                << err.message().c_str() << endl;
        }
    }

    else if(command[0] == "touch") {
        if(isNotMatch(command.size(), 2)) return;
        if(fs::exists(command[1])) { // .shct 파일이 존재할 때
            cout << command[1] + "File is already existent\n";
            return;
        }
        ofstream shctFile(command[1]); // 파일 생성 및 쓰기
        shctFile.flush();
        shctFile.close();
    }

    else if(command[0] == "cd") { // cd
        if(isNotMatch(command.size(), 2)) return; // 인자가 2개가 아닐 경우

        static Stack stack{-1}; // 정적으로 static 구조체 생성

        if(command[1][0] == '-') { // 옵션을 인자로 준 경우

            if(command[1] == "-pop") { // 옵션이 -pop이라면

                if(stack.top == -1) { // 스택이 빈 경우
                    cout << "CD stack is empty\n";
                    return;
                }
                fs::current_path(stack.stack[stack.top--]);
            }
            else if(command[1] == "-history") // 옵션이 -history라면
                for(int i = 0; i <= stack.top; i++) cout << i << " : " << stack.stack[i] << endl;
        }
        else if(!fs::is_directory(command[1], err)) { // 폴더가 존재하지 않는다면
            cout << "No such file or directory, error: " << err.message().c_str() << endl;
            return;
        }
        else { // 폴더가 존재한다면
            if(stack.top == MAX - 1) cout << "CD stack is full\n"; // 스택이 찬 경우
            else stack.stack[++stack.top] = fs::current_path(); // cd로 이동한 경로들을 stack 추가
            fs::current_path(command[1]);
        }
    }

    else if(command[0] == "find") { // find 명령어
        if(isNotMatch(command.size(), 4)) return;
        if(!fs::exists(command[1], err)) {
            cout << "No such file or directory, error: " << err.message().c_str() << endl;
            return;
        }
        if(command[2][0] == '-') {
            if(command[2] == "-name") {
                findFileAsName(command[1], command[3]);
            } else {
                cout << "No such option\n";
            }
        }
    }

    else if(command[0] == (os == "linux" ? "clear" : os == "window" ? "cls" : "clear")) { // clear 명령어
        if(isNotMatch(command.size(), 1)) return;
        if(os == "linux") system("clear");
        else if(os == "windows") system("cls");
        else system("clear");
    }
    currentDirectory = fs::current_path();
}