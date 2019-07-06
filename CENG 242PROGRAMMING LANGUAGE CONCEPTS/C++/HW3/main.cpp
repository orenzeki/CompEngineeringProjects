#include <iostream>
#include "Laptime.h"
#include <sstream>
#include "Car.h"
#include "Utilizer.h"
#include "Race.h"
#include "Championship.h"

bool VERBOSE = false;
int FAIL_COUNT = 0;
int TOTAL_FAILS = 0;

#define HEADER(x) ((VERBOSE) ? (_HEADER(x)) : (std::cout << ""))
#define FOOTER ((VERBOSE) ? (_FOOTER) : (std::cout << ""))
#define TESTHEADER(x) ((VERBOSE ? (_TESTHEADER(x)) : (std::cout << "")))
#define TESTFOOTER ((VERBOSE ? (_TESTFOOTER) : (std::cout << "")))
#define TESTRESULT(x) ((VERBOSE ? (_TESTRESULT(x)) : (std::cout << "")))
#define TESTNAME(x) ((VERBOSE ? (_TESTNAME(x)) : (std::cout << "")))
#define PRINT(x) (std::cout << x << std::endl)
#define _HEADER(x) (PRINT("=========================== " << x <<" ============================================"))
#define _FOOTER (PRINT("*******************************************************************************************"))
#define _TESTHEADER(x) (PRINT(x))
#define _TESTNAME(x) (PRINT("\t" << x))
#define _TESTFOOTER (PRINT("~~~~~~~~~~"))
#define _TESTRESULT(x) (PRINT("\t\t"<< x))
#define TEST(x, y) (TESTRESULT("TEST "<< x << " --> " << y))


void printResult(bool condition) {
    static int testNum = 1;
    if (condition) {
        TEST(testNum, "PASSED");
    } else {
        TEST(testNum, "FAILED");
        FAIL_COUNT++;
    }
    testNum++;
}

void laptimeTests() {
    HEADER("LAPTIME TESTS");
    TESTHEADER("CONSTRUCTOR TESTS");
    Laptime lt1 = Laptime(0);
    printResult(lt1.getLaptime() == 0);
    Laptime lt2 = Laptime(2);
    printResult(lt2.getLaptime() == 2);


    TESTHEADER("addLaptime TESTS");
    Laptime lt6 = Laptime(5);
    Laptime lt13 = Laptime(13);
    lt6.addLaptime(&lt2);
    lt2.addLaptime(&lt13);
    printResult(lt6.getNext() == &lt2 && lt6.getNext()->getNext() == &lt13);


    TESTHEADER("COPY CONSTRUCTOR TESTS");
    Laptime lt3 = Laptime(5);
    Laptime lt4 = Laptime(8);
    lt3.addLaptime(&lt4);
    Laptime lt5 = lt3;
    printResult((lt3.getLaptime() == lt5.getLaptime() && lt3.getNext() == lt5.getNext()));


    TESTHEADER("COMPARISON TESTS");
    TESTNAME("GREATER THAN");
    printResult(lt4 > lt3);
    printResult(!(lt3 > lt4));
    TESTNAME("LESS THAN");
    printResult(lt3 < lt4);
    printResult(!(lt4 < lt3));


    TESTHEADER("PLUS TEST");
    Laptime lt7 = Laptime(10);
    Laptime lt8 = Laptime(14);
    Laptime lt9 = Laptime(20);
    lt8.addLaptime(&lt9);
    lt7.addLaptime(&lt8);
    Laptime &lt12 = lt7 + lt8;
    printResult(lt12.getLaptime() == 24);


    TESTHEADER("CONVERT TO TIME TEST");
    Laptime lt10 = (50);
    Laptime lt11 = (60 * 1000 + 5 * 1000 + 500);
    std::stringstream out1;
    std::stringstream out2;
    out1 << lt10;
    printResult(out1.str() == "0:00.050");
    out2 << lt11;
    printResult(out2.str() == "1:05.500");


    if (FAIL_COUNT > 0)
        PRINT("YOU HAVE FAILED IN " << FAIL_COUNT << " TESTS IN LAPTIME");

    TOTAL_FAILS += FAIL_COUNT;
    FAIL_COUNT = 0;
    FOOTER;
}

void carTests() {
    HEADER("CAR TESTS");
    TESTHEADER("CONSTRUCTOR TESTS");
    Car car1 = Car("name1 surname1");
    printResult(car1.getDriverName() == "name1 surname1" && car1.getPerformance() > 0);


    TESTHEADER("addCar TESTS");
    Car car2 = Car("name2 surname2");
    Car car3 = Car("name3 surname3");
    car1.addCar(&car2);
    car2.addCar(&car3);
    printResult(car1.getNext() == &car2 && car1.getNext()->getNext() == &car3);


    TESTHEADER("Lap TESTS");
    Car car4 = Car("name4 surname4");
    int average = Utilizer::generateAverageLaptime();
    car4.Lap(average);
    car4.Lap(average);
    car4.Lap(average);
    cout << *car4.getLaptimeHead()<<endl;
    cout << *car4.getLaptimeHead()->getNext()<<endl;
    printResult(car4.getLaptimeHead()->getLength() == 3);

    TESTHEADER("INDEXING TESTS");
    TESTNAME("EXISTING");
    printResult(car4[0] == *car4.getLaptimeHead());
    printResult(car4[1] == *(car4.getLaptimeHead()->getNext()));
    TESTNAME("NONEXISTING");
    printResult(car4[5] == Laptime(0));


    TESTHEADER("COPY CONSTRUCTOR TESTS");
    Car car4_cp = Car(car4);
    printResult(car4.getDriverName() == car4_cp.getDriverName() &&
                car4.getLaptimeHead()->getLaptime() == car4_cp.getLaptimeHead()->getLaptime() &&
                car4.getLaptimeHead()->getNext()->getLaptime() == car4_cp.getLaptimeHead()->getNext()->getLaptime() &&
                car4.getLaptimeHead()->getNext()->getNext()->getLaptime() ==
                car4_cp.getLaptimeHead()->getNext()->getNext()->getLaptime()
    );

    Car car5 = Car("name5 surname5");
    Car car5_cp = Car(car5);
    printResult(car5.getDriverName() == car5_cp.getDriverName() &&
                car5.getNext() == car5_cp.getNext() &&
                car5.getLaptimeHead() == car5_cp.getLaptimeHead()
    );


    TESTHEADER("COMPARISON TESTS");
    TESTNAME("GREATER THAN");
    printResult(car4 > car5);
    TESTNAME("LESS THAN");
    printResult(car5 < car4);


    TESTHEADER("STREAM OVERLOAD TEST");
    Car car6 = Car("tester programminglanguagesconcept");
    car6.Lap(average);
    car6.Lap(average);
    car6.Lap(average);
    std::stringstream out;
    out << car6;
    printResult(out.str()[0] == 'P' && out.str()[1] == 'R' && out.str()[2] == 'O' &&
                out.str()[3] == '-' && out.str()[4] == '-' &&
                out.str()[13] == '-' && out.str()[14] == '-' &&
                out.str()[23] == '-' && out.str()[24] == '-');


    if (FAIL_COUNT > 0)
        PRINT("YOU HAVE FAILED IN " << FAIL_COUNT << " TESTS IN CAR");

    TOTAL_FAILS += FAIL_COUNT;
    FAIL_COUNT = 0;
    FOOTER;
}

/*void raceTests() {
    HEADER("RACE TESTS");
    TESTHEADER("CONSTRUCTOR TEST");
    Race r1 = Race("test");
    printResult(r1.getRaceName() == "test");

    TESTHEADER("addCartoRace TEST");
    Race r2 = Race("test");
    Car *car1 = new Car("testname testsurname");
    r2.addCartoRace(*car1);
    printResult(r2.getNumberOfCarsinRace() == 1 &&
                r2[0].getDriverName() == car1->getDriverName());
}
    TESTHEADER("INDEXING TESTS");
    TESTNAME("POSITION");
    printResult(r2[0].getDriverName() == car1->getDriverName());
    TESTNAME("NAME");
    printResult(r2["testname testsurname"].getDriverName() == car1->getDriverName());

    TESTHEADER("Lap TESTS");
    TESTNAME("++");
    Race r3 = Race("test");
    Car *c1 = new Car("hello aaab");
    Car *c2 = new Car("hello bbbc");
    Car *c3 = new Car("hello cccd");
    r3.addCartoRace(*c1);
    r3.addCartoRace(*c2);
    r3.addCartoRace(*c3);
    ++r3;
    TESTNAME("--");
    --r3;

    TESTHEADER("STREAM OPERATOR TEST");
    PRINT("\tCHECK IT WITH YOUR EYES");
    ++r3;
    ++r3;
    ++r3;
    ++r3;
    std::cout << r3;

    TESTHEADER("COPY CONSTRUCTOR TEST");
    //todo

    TESTHEADER("ASSIGNMENT OPERATOR TEST");
    //todo

    TESTHEADER("goBacktoLap TEST");
    std::stringstream out1, out2;
    out1 << r3;
    ++r3;
    r3.goBacktoLap(r3.getLapCount() - 2);
    out2 << r3;
    printResult(out1.str() == out2.str());

    if (FAIL_COUNT > 0)
        PRINT("\n" << "YOU HAVE FAILED IN " << FAIL_COUNT << " TESTS IN RACE" << "\n");

    TOTAL_FAILS += FAIL_COUNT;
    FAIL_COUNT = 0;
    FOOTER;
}

void championshipTests() {
    HEADER("CHAMPIONSHIP TESTS");
    TESTHEADER("ADD NEW RACE TEST");
    Race r1 = Race("test");
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    Championship cs1 = Championship();
    cs1.addNewRace(r1);
    cs1.addNewRace("test2");
    cs1.addNewRace("test3");
    printResult(cs1.getRaces().size() == 3);
    TESTHEADER("INDEXING TESTS");
    printResult(cs1["test"].getRaceName() == "test" &&
                cs1["test2"].getRaceName() == "test2" &&
                cs1["test3"].getRaceName() == "test3");
    TESTHEADER("ADD LAP TESTS");
    cs1.addLap("test");
    cs1.addLap("test");
    cs1.addLap("test");
    cs1.addLap("test2");
    cs1.addLap("test2");
    cs1.addLap("test3");
    Race race = cs1["test"];
    TESTNAME("NO TESTS HERE");
    /*
    printResult(cs1["test"].getLapCount() == 3 &&
                cs1["test2"].getLapCount() == 2 &&
                cs1["test3"].getLapCount() == 1);
                */
/*
    TESTHEADER("STREAM OPERATOR TEST");
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    r1.addCartoRace();
    Championship cs2 = Championship();
    cs2.addNewRace(r1);
    cs2.addNewRace("test4");
    cs2.addNewRace("test5");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");
    cs2.addLap("test");

    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");
    cs2.addLap("test4");

    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");
    cs2.addLap("test5");

    PRINT("\tCHECK WITH YOUR EYES");
    std::cout << cs2;

    TESTHEADER("REMOVE RACE TEST");
    cs1.removeRace("test");
    cs1.removeRace("test2");
    cs1.removeRace("test3");
    printResult(cs1.getRaces().size() == 0);

    if (FAIL_COUNT > 0)
        PRINT("\n" << "YOU HAVE FAILED IN " << FAIL_COUNT << " TESTS IN CHAMPIONSHIP" << "\n");

    TOTAL_FAILS += FAIL_COUNT;
    FAIL_COUNT = 0;
    FOOTER;
}*/

int main(){
    VERBOSE = true;
    laptimeTests();
    carTests();
    /*raceTests();
    /*championshipTests();*/

    if (TOTAL_FAILS > 0)
        PRINT("\n" << "YOU HAVE FAILED IN TOTAL NUMBER OF " << TOTAL_FAILS << " TESTS" << "\n");
    else
        PRINT ("\n" << "YOU PASSED ALL TESTS. CONGRATZ." << "\n");
    return 0;
}
