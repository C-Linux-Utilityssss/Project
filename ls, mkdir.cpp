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
    /* ?몄옄媛 3媛쒖씤 寃쎌슦 .shct ?뚯씪??留뚮벀
    ?몄옄???쒖꽌?濡?(shortcut <.shct ?뚯씪 ?대쫫> <寃쎈줈>) */
    if(command.size() == 3) {
        if(!fs::exists(command[2])) { // ?뚯씪??議댁옱?섏? ?딆쓣 ??
            cout << "No such file or directory\n";
            return;
        }
        if(fs::exists(command[1] + ".shct")) {
            cout << command[1] + ".shct file is already existent\n";
            return;
        }
        ofstream shctFile(command[1] + ".shct"); // ?뚯씪 ?앹꽦 諛??곌린
        fs::path absolutePath = command[2]; // 2踰덉㎏ ?몄옄 path 媛앹껜 ?뺥깭濡?媛?몄삤湲?
        shctFile << fs::canonical(absolutePath).string(); // ?덈?寃쎈줈濡??섑??닿린
        shctFile.flush();
        shctFile.close();
    // ?몄옄媛 2媛쒖씤 寃쎌슦 .shct ?뚯씪???곹엺 寃쎈줈濡??대룞
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
    // ?몄옄??媛쒖닔媛 留욎? ?딆쑝硫??먮윭硫붿꽭吏瑜??대낫??
    } else {
        cout << "The number of arguments does not match\n";
    }
}

/*----------------------------------*/
bool CreateDirectoryRecuresive(const std::string & dirName)
{
    error_code err;
    if (!fs::create_directories(dirName, err))
    {
        if (fs::exists(dirName))
        {
            return true;    // the folder probably already existed
        }

        printf("CreateDirectoryRecuresive: FAILED to create [%s], err:%s\n", dirName.c_str(), err.message().c_str());
        return false;
    }
    return true;
}

void runCommand(vector<string> command, fs::path& currentDirectory){
   if(command[0] == "ls"){
       for (const auto & entry : fs::directory_iterator(currentDirectory))
           cout << entry.path() << endl;
   }
   else if(command[0] == "mkdir"){
      CreateDirectoryRecuresive(command[1]);
   }

   currentDirectory = fs::current_path();
}