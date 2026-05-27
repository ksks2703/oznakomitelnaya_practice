#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
using namespace std;
struct aircraft{
    string model;
    string time;
    string number;
    int airfield;
};
/***************************************************************************************/
/*                                    Т Е С Т Ы                                        */
/***************************************************************************************/
//test0.txt - файл отсутствует
//test1.txt - пустой
//test2.txt - все 24 корректные 
//test3.txt - все 15 корректные 
//test4.txt - все 11 корректные
//test5.txt - только 1 самолет
//test6.txt - мусор + 1 самолет
//test7.txt - 7 некорректных, пустая строка,  4 корректных
//test8.txt - все 5 ошибки
//test9.txt - 2 кор + 5 некор
//test10.txt - пустые строки и мусор
//test11.txt - нехватка полей+кор+некор
//test12.txt - присутсвуют русские буквы+кор
//test13.txt - 1 аэродром(сортировка времени)
//test14.txt - ошибка времени везде
//test15.txt - ошибка марки везде
//test16.txt - ошибка номера везде(случаи 'O' вместо 0)
//test17.txt - ошибка аэродрома везде (случай 'З' вместо 3)
//test18.txt - 1 ошибка борт копия + 42 корректных
//test19.txt - на запас, если что то надо исправить
//test20.txt - на запас, если что то надо исправить

const string FILENAME = "test1.txt";
/***************************************************************************************/
/*                         П Р О Т О Т И П Ы    Ф У Н К Ц И Й                          */
/***************************************************************************************/
void errors(int er);//определение типа ошибок
bool isValidTime(const string&time); //проверка времени
bool isValidNumber(const string&number);//проверка номера
bool isValidModel(const string&model);//проверка модели
bool isValidAirfield(const int & airfield); //проверка посадки
void sorted(aircraft* planes,int count, int ind[]);//сортировка
int openfile(aircraft*&planes);//открытие файла
void printTable(aircraft* planes, int count,int* ind);//печать исходных данных
bool isUniqueNumber(aircraft* temp, int count, const string& number, const string& model);//уникальность ботового номера

/***************************************************************************************/
/*                         О С Н О В Н А Я     П Р О Г Р А М М А                       */
/***************************************************************************************/
int main(){
    aircraft * planes = nullptr;
    int count = openfile(planes);
    if (count == 0){
        cout<<"No data";
        delete[] planes;
        return 1;
    }
    int* ind = new int[count];
    sorted(planes,count,ind);
    printTable(planes,count,ind);
    delete[] planes;
    delete[] ind;
    return 0;
}
/***************************************************************************************/
/*                         Р Е А Л И З А Ц И Я    Ф У Н К Ц И Й                       */
/***************************************************************************************/

void errors(int er){ //версия на английском, мало ли
    switch (er){
        case 1:
            cout << "Error: Invalid flight time - ";
            break;
        case 2:
            cout << "Error: Invalid tail number - ";
            break;
        case 3:
            cout << "Error: Invalid aircraft model - ";
            break;
        case 4:
            cout << "Error: Invalid airfield number - ";
            break;
        case 5:
            cout << "Error: Cannot open file - ";
            break;
        case 6:
            cout << "Error: Duplicate tail number with different model - ";
            break;
    }
}

bool isUniqueNumber(aircraft* temp, int count, const string& number, const string& model) {
    for (int i = 0; i < count; i++) {
        if (temp[i].number == number && temp[i].model != model) {
            errors(6);
            return false;
        }
    }
    return true;
}
bool isValidTime(const string&time){ //проверка времени
    if (time.length()!=5 ||time[2]!=':'){
        errors(1);
        return false;
    }
    for (int t = 0; t<5;t++){
        if (t == 2) continue;
        if (!isdigit(time[t])){
            errors(1);
            return false;     
        }
    }
    int hours=(time[0]-'0')*10+(time[1]-'0');
    int minutes=(time[3]-'0')*10+(time[4]-'0');
    if (hours>23 || minutes>59){
        errors(1);
        return false;
    }
    return true;
}
bool isValidNumber(const string&number){
    if (number.length()!=6 || number[1]!='-' || number[0]<'A'|| number[0]>'Z'){
        errors(2);
        return false;
    }
    for (int n = 2; n<6;n++){
        if (!isdigit(number[n])){
            errors(2);
            return false;
        }
    }
    return true;
}
bool isValidModel(const string&model){
    if (model.length()!=7 || model[2]!='-' || model[6]<'A'|| model[6]>'Z'){
        errors(3);
        return false;
    }
    for (int m = 0; m<6;m++){
        if (m==2) continue;
        if (m == 0 || m == 1){
            if (model[m]<'A'|| model[m]>'Z'){
                errors(3);
                return false;
            }
        }
        if (m!=0 && m!=1){
            if (!isdigit(model[m])){
                errors(3);
                return false;
            }
        }
    }
    return true;
}
bool isValidAirfield(const int & airfield){
    if (airfield != 1 && airfield != 2 && airfield != 3){
        errors(4);
        return false;
    }
    return true;
}
int openfile(aircraft*& planes) {
    ifstream file(FILENAME);
    if (!file.is_open()) {
        errors(5);
        cout << " File: " << FILENAME << endl;
        return 0;
    }
    
    aircraft temp[100];
    int count = 0;
    string line;
    
    while (getline(file, line) && count < 100) {
        if (line.empty()) continue;
        
        aircraft a;
        stringstream ss(line);
        ss >> a.model >> a.number >> a.time >> a.airfield;
        
        if (ss.fail()) {
            cout<<"Error reading line: "<<line<<endl;
            continue;
        }
        
        // Проверяем и при ошибке выводим всю строку
        if (!isValidModel(a.model) || !isValidNumber(a.number) || 
            !isValidTime(a.time) || !isValidAirfield(a.airfield)) {
            cout<<"in line: "<<line<<endl;
            continue;
        }
        if (!isUniqueNumber(temp, count, a.number, a.model)) {
            cout<<"in line: "<<line<<endl;
            continue;
        }
        
        temp[count++] = a;
    }
    
    file.close();
    if (count == 0) return 0;
    
    planes = new aircraft[count];
    for (int i = 0; i < count; i++) {
        planes[i] = temp[i];
    }
    
    return count;
}
void sorted(aircraft * planes,int count, int* ind){
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
void printTable(aircraft* planes, int count,int* ind){
    for(int i = 1; i<=3;i++){
        cout << "\t\t\t\t\t\tAIRFIELD " << i << "\n";
        cout << "MODEL\tNUMBER\tTIME\n";
        for(int j = 0;j<count;j+=1){
            int id=ind[j];
            if (planes[id].airfield == i) cout<<planes[id].model<<"\t"<<planes[id].number<<"\t"<<planes[id].time<<endl;

        }
    }
}
