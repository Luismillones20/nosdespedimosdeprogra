#include <iostream>
#include <fstream>
using namespace std;
int main(){
    ifstream file("C:/progra3/mpst_full_data (1).csv");
    if (!file){
        cerr << "Error file open " << endl;
    }
    string id;
    string title;
    string plot_synopsis;
    string tags;
    string split;
    string synopsis_source;
    string separador;
    string problem = "";
    int count = 0;
    while (count < 100){
        if (count == 0){
            getline(file, id, ',');
            getline(file, title, ',');
            getline(file, plot_synopsis, ',');
            getline(file, tags, ',');
            getline(file, split,',');
            getline(file, synopsis_source, '\n');
            cout << "ID: " << id << endl;
            cout << "Titulo: " << title << endl;
            cout << "Sinopsis: " << plot_synopsis << endl;
            cout << "tags: " << tags << endl;
            cout << "split: " << split << endl;
            cout << "synopsis_source: " << synopsis_source << endl;
            cout << "-------------------------------------------------------------------------------------------------------------" << endl << endl;
            count++;
        }
        getline(file, id, ',');
        getline(file, title, ',');
        getline(file, separador, '"');
        cout << "ID: " << id << endl;
        cout << "Titulo: " << title << endl;
        cout << "Sinopsis: " ;
        while (problem.size() == 0 ){
            getline(file, plot_synopsis, '"');
            getline(file, problem, '"');
            cout << plot_synopsis; // falta poner las dobles comas
        }
        problem = "";
        cout << endl;
        getline(file, tags, '"');
        getline(file,separador,',');
        getline(file, split,',');
        getline(file, synopsis_source, '\n');

        cout << "tags: " << tags << endl;
        cout << "split: " << split << endl;
        cout << "synopsis_source: " << synopsis_source << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------" << endl << endl;
        count++;
    }
}