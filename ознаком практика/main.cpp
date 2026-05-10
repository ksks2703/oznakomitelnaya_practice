#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <sstream>
using namespace std;
struct aircraft{
    string model;
    string time;
    string number;
    int airfield;
};

// void errors(int er){            //определение типа ошибок
//     switch (er){
//         case 1:
//             cout<<"Ошибка времени самолета\n";
//             break;
//         case 2:
//             cout<<"Ошибка бортового номера самолета\n";
//             break;
//         case 3:
//             cout<<"Ошибка марки самолета\n";
//             break;
//         case 4:
//             cout<<"Ошибка АП самолета\n";
//             break;
//         case 5:
//             cout<<"Ошибка: не удалось открыть файл";
//             break;
//     }
// }
bool isValidTime(const string&time){ //проверка времени
    if (time.length()!=5 ||time[2]!=':'){
        //errors(1);
        return false;
    }
    for (int t = 0; t<5;t++){
        if (t == 2) continue;
        if (!isdigit(time[t])){
            //errors(1);
            return false;     
        }
    }
    int hours=stoi(time.substr(0,2));
    int minutes=stoi(time.substr(3,2));
    if (hours>23 || hours<0 || minutes>59 || minutes<0){
        //errors(1);
        return false;
    }
    return true;
}
bool isValidNumber(const string&number){
    if (number.length()!=6 || number[1]!='-' || number[0]<'A'|| number[0]>'Z'){
        //errors(2);
        return false;
    }
    for (int n = 2; n<6;n++){
        if (!isdigit(number[n])){
            //errors(2);
            return false;
        }
    }
    return true;
}
bool isValidModel(const string&model){
    if (model.length()!=7 || model[2]!='-' || model[6]<'A'|| model[6]>'Z'){
        //errors(3);
        return false;
    }
    for (int m = 0; m<6;m++){
        if (m==2) continue;
        if (m == 0 || m == 1){
            if (model[m]<'A'|| model[m]>'Z'){
                //errors(3);
                return false;
            }
        }
        if (m!=0 && m!=1){
            if (!isdigit(model[m])){
                //errors(3);
                return false;
            }
        }
    }
    return true;
}
bool isValidAirfield(const int & airfield){
    if (airfield != 1 && airfield != 2 && airfield != 3){
        //errors(4);
        return false;
    }
    return true;
}
int openfile(aircraft planes[], int maxSize) {
    ifstream file("test2.txt");
    if (!file.is_open()) {
        return 0;
    }
    
    int count = 0;
    string line;
    
    while (getline(file, line) && count < maxSize) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        aircraft a;
        ss >> a.model >> a.number >> a.time >> a.airfield;
        if (ss.fail()) continue;  // не хватает полей
        
        if (isValidModel(a.model) && isValidNumber(a.number) && 
            isValidTime(a.time) && isValidAirfield(a.airfield)) {
            planes[count] = a;
            count++;
        }
    }
    
    file.close();
    return count;
}
void sorted(aircraft planes[],int count, int ind[]){
    for (int i = 0;i<count;i++){
        ind[i]= i;
    }
    int buf;
    for (int i = 0;i<count-1;i++){
        int maxx = i;
        for (int j = i+1;j<count;j++){
            if (planes[ind[j]].time>planes[ind[maxx]].time){
                maxx=j;
            }
        }
        buf = ind[i];
        ind[i]=ind[maxx];
        ind[maxx]=buf;
    }
}
void printTable(aircraft planes[], int count,int ind[]){
    for(int i = 1; i<=3;i++){
        cout<<"\t\t\t\t\t\tАЭРОДРОМ "<<i<<"\n";
        cout<<"МАРКА\tБорт\tВремя\n";
        for(int j = 0;j<count;j+=1){
            int id=ind[j];
            if (planes[id].airfield == i){
                cout<<planes[id].model<<"\t"<<planes[id].number<<"\t"<<planes[id].time<<endl;
            }
        }
    }
}
int main(){
    setlocale(LC_ALL,"rus");
    const int MAX = 100;
    aircraft planes[MAX];
    int count = openfile(planes,MAX);
    if (count == 0){
        cout<<"Нет данных";
        return 1;
    }
    int ind[MAX];
    sorted(planes,count,ind);
    printTable(planes,count,ind);
    return 0;
}