#include "Laptime.h"
#include "Car.h"

#include <iostream>

using namespace std;


int main(){

    Laptime l1(400);
    
    Laptime l11(500);
    Laptime l12(600);
    Laptime* l2 = new Laptime(300);
    Laptime* l3 = new Laptime(200);
    Laptime* l13 = new Laptime(600);
    Laptime* l14 = new Laptime(600);
    l1.addLaptime(l2);
    l1.addLaptime(l3);
    //l1 = l11;
    cout << "l1: ";
    cout << l1 << endl;
    cout << "l1.display(): ";
    l1.display();
    cout << "l1=l11: ";
    l1=l11;
    l1.display();

    l11.addLaptime(l13);
    l11.addLaptime(l2);
    l1.addLaptime(l2);
    cout << "l11: ";
    l11.display();
    cout << "l1: ";
    l1.display();

    Laptime l5(42);
    Laptime l4(32);

    cout << "l5+l4: ";
    cout << l5 + l4 << endl;
    cout << "l1.length(): ";
    cout << l1.getLength()<<endl;
    /*cout << "sumLaptime: ";
    l1.sumLaptime();
    cout << "bestLaptime: ";
    l1.bestLaptime();
    cout << "lastLaptime: ";
    l1.lastLaptime();*/

    cout << "getlap: ";
    cout << l1.getValue() << endl;
    cout << "l5<l4: ";
    cout << (l5 < l4) << endl;
    cout << "l5>l4: ";
    cout << (l5 > l4) << endl << endl;

    Car alonso("Fernando Alonso");
    Car vettel("Sebastian Vettel");
    Car hamilton("Lewis Hamilton");
    Car* raik = new Car("kimi raikkonen");
    Car* anan = new Car("kimi anakkonen");

    
    alonso.Lap(l1);
    
    alonso.Lap(l11);
    
    cout << "alonso: ";
    cout << alonso <<endl;
    
    cout << "car indexing:";
    cout << alonso[0] << endl;

    cout << "vettel = alonso: ";
    vettel=alonso;
    cout << vettel << endl;

    cout << "getDriverName: ";
    cout << hamilton.getDriverName() << endl;

    cout << "getPerformance: ";
    cout << hamilton.getPerformance() << endl;

    hamilton.Lap(l4);
    hamilton.Lap(l5);
    hamilton.Lap(l1);

    raik->Lap(l4);
    raik->Lap(l5);
    raik->Lap(l1);

    anan->Lap(l4);
    anan->Lap(l5);
    anan->Lap(l1);
    cout << "hamilton: ";
    cout << hamilton << endl;

    cout << "hamilton < alonso: ";
    cout << (hamilton < alonso) << endl;

    cout << "hamilton > alonso: ";
    cout << (hamilton > alonso) << endl;

    cout << "raikkonen: ";
    cout << *raik << endl;

    alonso.addCar(raik);
    alonso.addCar(anan);
    cout<<"display(): "<<endl;
    alonso.display();
// try addCar1!!!1!!!!!!!!!!!!
// fix big 3 and others like laptime!!!!!!!!!!!!!   

    return 0;

}
