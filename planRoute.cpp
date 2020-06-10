#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct Planet{
    int row;
    int col;
    string symbol;
    string id;
    string name;
};
// changes the name of the planet if there is XX or _
void transmissionError(string &name){
    while(name.find("XX")!= name.npos){
        //finds the index and replaces with empty string
        int ind = name.find("XX");
        name.replace(ind, 2, "");
    }
    while(name.find("_")!= name.npos){
        //finds the index and replaces with regular space
        int ind = name.find("_");
        name.replace(ind, 1," ");
    }
}

//traversing the vector travelled to see if the ID is in it 
bool travelledAlready(string ID, vector<string> &travelled){
    for(int i = 0; i < travelled.size(); ++i){
        if(travelled.at(i) == ID){
            return true;
        }
    }
    return false;
}

//finding and returning the ID of the planet that is closest
string bestRoute(int row, int col, int erow, int ecol, vector<Planet> newPlanets, vector<string> &travelled){
    double closest = erow * ecol* 10000; //a big number to compare to
    string closestPlanet;
    for(int i = 0; i< newPlanets.size(); ++i){
        double dist = pow(pow(row - newPlanets.at(i).row,2) + pow(col- newPlanets.at(i).col,2),0.5); //distance formula
        if(dist< closest && !travelledAlready(newPlanets.at(i).id, travelled) ){
            closest = dist; //if it is smaller than the closest than set closest to the smaller value and save the id
            closestPlanet = newPlanets.at(i).id;
        }
    }
    travelled.push_back(closestPlanet); //add the new planet into the travelled planets vector to keep track
    return closestPlanet;
}

int main(){
    string lfile, nfile;
    int rows, cols, srow, scol, erow, ecol;
    vector<Planet> planets;
    vector<Planet> newPlanets;
    vector<string> travelled;

    cout<< "Enter Locations Filename: "; //enter in filenames and storing them
    cin >> lfile;
    cout << "Enter Names Filename: ";
    cin>> nfile;

    ifstream locations(lfile);//opening the files
    ifstream names(nfile);

    if(!locations.is_open() || !names.is_open()){ //seeing if there is an error opening the files
        cout << "Input file could not be opened"<< endl;
        return 1;
    }

    locations>> rows >> cols >> srow >> scol >> erow >> ecol;
    Planet temp;
    while(locations >> temp.row >>temp.col >> temp.symbol >> temp.id){
        planets.push_back(temp); //read in all the planet data and add to vector
    }

    while(names >> temp.id >> temp.name){
        for(int i = 0; i < planets.size(); ++i){
            if(planets.at(i).id == temp.id){ //matching up the id in name file and adding it to its respective planet
              planets.at(i).name = temp.name;  
            }
        }
    }

    for(int i = 0; i < planets.size(); ++i){
        if(planets.at(i).name.find("XX") !=  planets.at(i).name.npos|| planets.at(i).name.find("_")!=  planets.at(i).name.npos){
            transmissionError(planets.at(i).name);
        }
        if(planets.at(i).row > rows || planets.at(i).col > cols || planets.at(i).row <= 0 || planets.at(i).col <= 0){
            cout << planets.at(i).id << " out of range - ignoring"<< endl; //getting rid of the planets that is out of bounds
        }
        else if (!(planets.at(i).row > rows || planets.at(i).col > cols)){
            newPlanets.push_back(planets.at(i)); //adding only the good planets into new vector to evaluate with
        }
    }

    locations.close();
    names.close();
    ofstream output("journey.txt");

    for(int rowi = 1; rowi <= rows; ++rowi){
        for(int coli = 1; coli <= cols ; ++coli){
            int printed = 0;//keeping track if printed a character at that position
            if(rowi == srow && coli == scol){
                output << "S";//finding the start
                printed = 1;
            }
            if (rowi == erow && coli == ecol){
                output<< "E";//finding the end
                printed = 1;
            }
            for(int i = 0; i < newPlanets.size(); ++i){
                if(newPlanets.at(i).row == rowi && newPlanets.at(i).col== coli){
                    output << newPlanets.at(i).symbol; //adding planets on map
                    printed = 1;
                }
            }
            if(printed == 0){
                output << "."; //everywhere else is a .
            }
        }
        output << endl;
    }

    output << "Start at "<< srow << " "<< scol<< endl;
    int currentRow = srow; //keeping start of the starting point 
    int currentCol = scol;


    while(travelled.size() <= newPlanets.size()){
        string tempID = bestRoute(currentRow, currentCol, erow, ecol,newPlanets, travelled);
        for(int i = 0; i < newPlanets.size(); ++i){
            if(newPlanets.at(i).id == tempID){ //if the id matches up with the next planet to go to then print it out
                output << "Go to "<< newPlanets.at(i).name << " at "<< newPlanets.at(i).row << " "<< newPlanets.at(i).col << endl;
                currentRow = newPlanets.at(i).row;
                currentCol = newPlanets.at(i).col;
            }
        }
    }

    output<< "End at "<< erow<< " "<< ecol<< endl; //print end
    output.close();
}