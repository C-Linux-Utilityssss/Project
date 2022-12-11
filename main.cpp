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

/*--------------------------------------------------------------*/

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
typedef struct Stack {
    int top;
    string stack[MAX];
} Stack;
/*--------------------------------------------------------------*/

void runCommand(vector<string> command, fs::path& currentDirectory){
    error_code err;
    if(command[0] == "ls") {
        int cnt = 0;
        int brCnt = 3;
        int max = 0;
        for (const auto& entry : fs::directory_iterator(currentDirectory)) {
            if(max < entry.path().string().length()) max = entry.path().string().length();
        }
        brCnt -= max / 60;
        for (const auto& entry : fs::directory_iterator(currentDirectory)) {
            if(fs::is_directory(entry.path())) {
                cout << getRightPaddingString(entry.path().string().substr(entry.path().string().find_last_of("/")), max, ' ') + '\t';
            } else {
                cout << getRightPaddingString(entry.path().string().substr(entry.path().string().find_last_of("/") + 1), max, ' ') + '\t';
            }
            cnt++;
            if(cnt % brCnt == 0) {
                cout << endl;
                cnt = 0;
            }
        }
        if(cnt) cout << endl;
    }
    else if(command[0] == "mkdir") {
        if(isNotMatch(command.size(), 2)) return;
        if(!fs::create_directories(command[1], err)) {
            cout << "Failed to create directory, error: " 
                << err.message().c_str() << endl;
        }
    }
    else if(command[0] == "cd") {
        static Stack stack{-1};
        if(isNotMatch(command.size(), 2)) return;
        if(command[1][0] == '-') { // 옵션
            if(command[1] == "-pop") {
                if(stack.top == -1) {
                    cout << "CD stack is empty\n";
                    return;
                }
                fs::current_path(stack.stack[stack.top--]);
            }
            else if(command[1] == "-history")
                for(int i = 0; i <= stack.top; i++) cout << i << " : " << stack.stack[i] << endl;
        }
        else if(!fs::is_directory(command[1], err)) { // 폴더가 존재하지 않는다면
            cout << "No such file or directory, error: " << err.message().c_str() << endl;
            return;
        }
        else { // 폴더가 존재한다면
            if(stack.top == MAX - 1) cout << "CD stack is full\n";
            else stack.stack[++stack.top] = fs::current_path(); // cd로 이동한 경로들을 stack에 저장함
            fs::current_path(command[1]);
        }
    }
    currentDirectory = fs::current_path();
}