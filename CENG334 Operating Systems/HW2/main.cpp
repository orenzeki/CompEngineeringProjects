#include <iostream>
#include "monitor.h"
#include <unistd.h>
#include <vector>
#include <string>
#include<pthread.h>
#include <bits/stdc++.h>

using namespace std;

class Person {

private:
    int personWeight;
    int initialFloor;
    int destinationFloor;
    int priority;
public:
    int personId;
    Person(int personId, int personWeight, int initialFloor, int destinationFloor, int priority) {
        this->personId = personId;
        this->personWeight = personWeight;
        this->initialFloor = initialFloor;
        this->destinationFloor = destinationFloor;
        this->priority = priority;
    }
    ~Person();

    int getPersonWeight() {
        return this->personWeight;
    }

    int getInitialFloor() {
        return this->initialFloor;
    }

    int getDestinationFloor() {
        return this->destinationFloor;
    }

    int getPriority() {
        return this->priority;
    }
};

Person::~Person() = default;


class ElevatorMonitor: public Monitor {
    int flag;
    int num_floors;
    int num_people;
    int weight_capacity;
    int person_capacity;
    int tt;
    int it;
    int iot;
    int requestNumber;
    int currentFloor;
    int currentWeight;
    int currentPeople;
    int served;
    int elevatorStopped;
    int destinationFloor;
    int direction;
    int control;
    int lock;
    int elevatorStatus;
    int exitFlag;
    int upDown;
    vector<Person> requests;
    vector<int> destination;
    vector<Person> willEnterHP;
    vector<Person> willEnterLP;
    vector<Person> enteredHP;
    vector<Person> enteredLP;
    vector<Person> enteredElevator;
private:
    Condition canRequest, canElevate;
public:
    ElevatorMonitor(int a, int b, int c, int d, int e, int f, int g) : canRequest(this), canElevate(this){
        num_floors = a;
        num_people = b;
        weight_capacity = c;
        person_capacity = d;
        tt = e;
        it = f;
        iot = g;
        requestNumber = 0;
        currentFloor = 0;
        currentWeight = 0;
        currentPeople = 0;
        flag = 0;
        served = 0;
        elevatorStopped = 1;
        destinationFloor = 0;
        direction = 0;
        exitFlag = 0;
        control = 0;
        lock = 0;
        elevatorStatus = 0;
        upDown = 0;
    }

    int getServed() const {
        return served;
    }

    int getNumPeople() const {
        return num_people;
    }

    ~ElevatorMonitor() = default;
    int request(Person* person) {
        __synchronized__;
        while(!elevatorStopped || lock){
            canRequest.wait();
        }

        for(auto & i : enteredElevator){
            if(person->personId == i.personId){
                return -1;
            }
        }
        for(auto & request : requests){
            if(person->personId == request.personId)
                flag = 1;
        }

        if(currentWeight+person->getPersonWeight() <= weight_capacity && currentPeople < person_capacity){
            if(!flag && (direction == 0 || (direction == 1 && person->getInitialFloor() >= currentFloor && person->getInitialFloor() < person->getDestinationFloor()) ||
                         (direction == 2 && person->getInitialFloor() <= currentFloor && person->getInitialFloor() > person->getDestinationFloor()))){
                requests.push_back(*person);
            }
            else{
                flag = 0;
                return person->personId;
            }
        }
        else{
            flag = 0;
            return person->personId;
        }

        printRequest(person);

        if(currentFloor != person->getInitialFloor()){
            addInitialFloor(person);
            showElevator();
        }
        else{
            if(person->getInitialFloor() < person->getDestinationFloor())
                printMoveUp();
            else
                printMoveDown();

        }



        canElevate.notify();
        requestNumber++;
        return person->personId;
    }

    void showElevator(){

        if(destination.empty()){
            cout << "Elevator (Idle, " << currentWeight << ", " << currentPeople << ", " << currentFloor << " ->)" << endl;
            direction = 0;

        }
        else if(currentFloor < destination[0]){
            cout << "Elevator (Moving-up, " << currentWeight << ", " << currentPeople << ", " << currentFloor << " -> ";
            for(int i=1; i<destination.size(); i++){
                cout << destination[i-1] << ", ";
            }
            cout << destination[destination.size()-1];
            cout << ')' << endl;
            direction = 1;
            upDown = 1;
        }
        else if(currentFloor > destination[destination.size()-1]){
            cout << "Elevator (Moving-down, " << currentWeight << ", " << currentPeople << ", " << currentFloor << " -> ";
            for(int i=1; i<destination.size(); i++){
                cout << destination[i-1] << ", ";
            }
            cout << destination[destination.size()-1];
            cout << ')' << endl;
            direction = 2;
            upDown = 2;
        }
        else{
            cout << "Elevator (Idle, " << currentWeight << ", " << currentPeople << ", " << currentFloor << " ->)" << endl;
            direction = 0;
        }

    }

    void addInitialFloor(Person* person){
        destination.push_back(person->getInitialFloor());
        sort(destination.begin(), destination.end());
        destination.erase( unique( destination.begin(), destination.end() ), destination.end() );
        destinationFloor = person->getInitialFloor();
    }

    void addDestinationFloor(Person* person){
        destination.push_back(person->getDestinationFloor());
        sort(destination.begin(), destination.end());
        destination.erase( unique( destination.begin(), destination.end() ), destination.end() );
        destinationFloor = person->getDestinationFloor();
    }

    void moveUpwards(){
        currentFloor++;
        if(currentFloor == destination[0]){
            destinationFloor = destination[0];
            destination.erase(destination.begin());
        }

        if(destination.empty()){
            elevatorStopped = 0;
            printIdle();
        }
        else{
            printMoveUp();
            destinationFloor = destination[0];
        }
    }

    void moveDownwards(){
        currentFloor--;
        if(currentFloor == destination[destination.size()-1]){
            destinationFloor = destination[destination.size()-1];
            destination.pop_back();
        }

        if(destination.empty()){
            elevatorStopped = 0;
            printIdle();
        }
        else{
            printMoveDown();
            destinationFloor = destination[destination.size()-1];
        }
    }

    void printMoveUp(){
        cout << "Elevator (Moving-up, " << currentWeight << ", " << currentPeople << ", " << currentFloor << " -> ";
        if(!destination.empty()){
            for(int i=0; i<destination.size()-1; i++){
                cout << destination[i] << ", ";
            }
            cout << destination[destination.size()-1];
        }
        cout << ')' << endl;
        direction = 1;
    }

    void printMoveDown(){
        cout << "Elevator (Moving-down, " << currentWeight << ", " << currentPeople << ", " << currentFloor << " -> ";
        if(!destination.empty()){
            for(int i=0; i<destination.size()-1; i++){
                cout << destination[i] << ", ";
            }
            cout << destination[destination.size()-1];
        }
        cout << ')' << endl;
        direction = 2;
    }

    void printIdle(){
        cout << "Elevator (Idle, " << currentWeight << ", " << currentPeople << ", " << currentFloor << " ->)" << endl;
        direction = 0;
    }

    void leftHP(Person* person){
        currentWeight -= person->getPersonWeight();
        currentPeople--;
        cout << "Person (" << person->personId << ", hp, " << person->getInitialFloor() << " -> " << person->getDestinationFloor()
             << ", " << person->getPersonWeight() << ") has left the elevator" << endl;
    }

    void leftLP(Person* person){
        currentWeight -= person->getPersonWeight();
        currentPeople--;
        cout << "Person (" << person->personId << ", lp, " << person->getInitialFloor() << " -> " << person->getDestinationFloor()
             << ", " << person->getPersonWeight() << ") has left the elevator" << endl;
    }

    void enterHP(Person* person){
        currentWeight += person->getPersonWeight();
        currentPeople++;
        cout << "Person (" << person->personId << ", hp, " << person->getInitialFloor() << " -> " << person->getDestinationFloor()
             << ", " << person->getPersonWeight() << ") entered the elevator" << endl;
    }

    void enterLP(Person* person){
        currentWeight += person->getPersonWeight();
        currentPeople++;
        cout << "Person (" << person->personId << ", lp, " << person->getInitialFloor() << " -> " << person->getDestinationFloor()
             << ", " << person->getPersonWeight() << ") entered the elevator" << endl;
    }

    void eraseRequest(Person* person){
        int deleted = 0;
        for(int i=0; i<requests.size()+deleted; i++){
            if(person->personId == requests[i].personId){
                requests.erase(requests.begin()+i);
                deleted++;
            }
        }
    }

    static void printRequest(Person* person){
        if (person->getPriority() == 1)
            cout << "Person (" << person->personId << ", hp, " << person->getInitialFloor()
                 << " ->" << person->getDestinationFloor() << ", " << person->getPersonWeight() << ") made a request" << endl;
        else
            cout << "Person (" << person->personId << ", lp, " << person->getInitialFloor()
                 << " ->" << person->getDestinationFloor() << ", " << person->getPersonWeight() << ") made a request" << endl;
    }

    void controlRequest(Person* person){
        if(person->getInitialFloor() < person->getDestinationFloor() && upDown == 2){
            printRequest(person);
            showElevator();
        }
        else if(person->getInitialFloor() > person->getDestinationFloor() && upDown == 1){
            printRequest(person);
            showElevator();
        }
    }


    void elevate() {
        __synchronized__;

        vector<Person> peopleExitHP;
        vector<Person> peopleExitLP;

        willEnterHP.clear();
        willEnterLP.clear();
        control = 0;

        usleep(it);
        while(!requestNumber){
            canElevate.wait();
        }
        for(int i=0; i<requests.size(); i++){
            if(currentFloor == requests[i].getInitialFloor())
                control = 1;

        }
        if (currentFloor < destinationFloor && !control){
            elevatorStopped = 0;
            usleep(tt);
            moveUpwards();
        }
        else if (currentFloor > destinationFloor && !control){
            elevatorStopped = 0;
            usleep(tt);
            moveDownwards();
        }
        for(int i=0; i<enteredHP.size(); i++){
            if(enteredHP[i].getDestinationFloor() == currentFloor){
                exitFlag =1;
                break;
            }
        }
        for(int i=0; i<enteredLP.size(); i++){
            if(enteredLP[i].getDestinationFloor() == currentFloor){
                exitFlag =1;
                break;
            }
        }


        if(currentFloor == destinationFloor || control || exitFlag){
            elevatorStopped = 1;
            int exitHP = 0;
            int exitLP = 0;
            for(int i=0; i<enteredHP.size()+exitHP; i++){
                if(currentFloor == enteredHP[i].getDestinationFloor() && enteredHP[i].getPriority() == 1){
                    peopleExitHP.push_back(enteredHP[i]);
                    enteredHP.erase(enteredHP.begin()+i);
                    exitHP++;
                }
            }
            for(int i=0; i<enteredLP.size()+exitLP; i++){
                if(currentFloor == enteredLP[i].getDestinationFloor() && enteredLP[i].getPriority() == 2){
                    peopleExitLP.push_back(enteredLP[i]);
                    enteredLP.erase(enteredLP.begin()+i);
                    exitLP++;
                }
            }

            for(auto & i : peopleExitHP){
                usleep(iot);
                leftHP(&i);
                peopleExitHP.erase(peopleExitHP.begin());
                showElevator();
                served++;
                elevatorStatus--;
                canRequest.notifyAll();
            }
            for(auto & i : peopleExitLP){
                usleep(iot);
                leftLP(&i);
                peopleExitLP.erase(peopleExitLP.begin());
                showElevator();
                served++;
                elevatorStatus--;
                canRequest.notifyAll();
            }
            exitFlag = 0;
        }

        if(currentFloor == destinationFloor || control){
            elevatorStopped = 1;
            for(auto & request : requests){
                if (currentFloor == request.getInitialFloor()) {
                    if ((request.getDestinationFloor() >= currentFloor && destinationFloor >= currentFloor) ||
                        (request.getDestinationFloor() <= currentFloor && destinationFloor <= currentFloor)) {
                        if (request.getPriority() == 1)
                            willEnterHP.push_back(request);
                        else
                            willEnterLP.push_back(request);

                    }
                    else{
                        if(currentFloor == request.getInitialFloor()){
                            for(int j=0; j<requests.size(); j++){
                                if(request.personId == requests[j].personId){
                                    requests.erase(requests.begin()+j);
                                    requestNumber--;
                                    j--;
                                }
                            }
                        }
                    }

                }
            }

            if(!willEnterHP.empty()){
                for(auto & i : willEnterHP){
                    if(currentWeight+i.getPersonWeight() <= weight_capacity && currentPeople < person_capacity){
                        controlRequest(&i);
                        usleep(iot);
                        enterHP(&i);
                        enteredElevator.push_back(i);
                        enteredHP.push_back(i);
                        addDestinationFloor(&i);
                        showElevator();
                        eraseRequest(&i);
                        elevatorStatus++;
                        canRequest.notifyAll();

                    }
                    else{
                        if(currentFloor == i.getInitialFloor()){
                            for(int j=0; j<requests.size(); j++){
                                if(i.personId == requests[j].personId){
                                    requests.erase(requests.begin()+j);
                                    requestNumber--;
                                    j--;
                                }
                            }
                        }
                    }

                }
            }
            if(!willEnterLP.empty()){
                for(auto & i : willEnterLP){
                    if(currentWeight+i.getPersonWeight() <= weight_capacity && currentPeople < person_capacity){
                        controlRequest(&i);
                        usleep(iot);
                        enterLP(&i);
                        enteredElevator.push_back(i);
                        enteredLP.push_back(i);
                        addDestinationFloor(&i);
                        showElevator();
                        eraseRequest(&i);
                        elevatorStatus++;
                        canRequest.notifyAll();
                    }
                    else{
                        if(currentFloor == i.getInitialFloor()){
                            for(int j=0; j<requests.size(); j++){
                                if(i.personId == requests[j].personId){
                                    requests.erase(requests.begin()+j);
                                    requestNumber--;
                                    j--;
                                }
                            }
                        }
                    }
                }
            }
            lock = 0;
        }

        canElevate.notify();
    }


};
struct ElParam {
    ElevatorMonitor *elMonitor;
    Person* person;
};

void *personFunction(void *args){

    auto *ep = (ElParam *) args;
    ElevatorMonitor *el = ep->elMonitor;
    Person* person = ep->person;

    while(true){
        int temp = el->request(person);
        if(temp < 0)break;
    }
    return nullptr;
}

void* elevatorFunction(void *args){
    auto *el = (ElevatorMonitor *) args;
    while(true){
        if(el->getServed() == el->getNumPeople()) break;
        el->elevate();
    }

    return nullptr;
}



int main(int argc, char** argv) {
    pthread_t *peopleThread, elevatorThread;
    vector<string> rows;
    string row;

    ifstream readFile;
    readFile.open(argv[1]);

    while(!readFile.eof()){

        getline(readFile,row);
        rows.push_back(row);

    }
    readFile.close();

    vector<int> values;
    string s;
    size_t position = 0;
    int x = 0;
    for(int i=0; i<7; i++){

        position = rows[0].find(' ');
        s = rows[0].substr(0,position);
        stringstream str(s);
        str >> x;
        values.push_back(x);
        rows[0].erase(0, position+1);

    }
    vector<vector<int>> people;
    for(int i=0; i<values[1]; i++){
        vector<int> temp;
        for(int j=0; j<4; j++){

            position = rows[i+1].find(' ');
            s = rows[i+1].substr(0,position);
            stringstream str(s);
            str >> x;
            temp.push_back(x);
            rows[i+1].erase(0,position+1);


        }
        people.push_back(temp);
    }

    auto *elParams = new ElParam[values[1]];
    ElevatorMonitor em(values[0], values[1], values[2], values[3], values[4], values[5], values[6]);
    peopleThread = new pthread_t[values[1]];
    for(int i=0; i<values[1]; i++){
        elParams[i].person = new Person(i, people[i][0], people[i][1], people[i][2], people[i][3]);
        elParams[i].elMonitor = &em;
        pthread_create(&peopleThread[i], nullptr, personFunction, (void *) (elParams + i));
    }

    pthread_create(&elevatorThread, nullptr, elevatorFunction, (void *) &em);

    for(int i=0; i<values[1]; i++){

        pthread_join(peopleThread[i], nullptr);

    }

    pthread_join(elevatorThread, nullptr);

    return 0;

}