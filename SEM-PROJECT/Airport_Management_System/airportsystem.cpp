#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <exception>
using namespace std;


class UserAuth;
class AirportUser;
class Passenger;
class Cargo;
class Staff;
class Flight;
class AirportSystem


const string FLIGHTS_FILE = "flights.txt";
const string PASSENGERS_FILE = "passengers.txt";
const string CARGO_FILE = "cargo.txt";
const string STAFF_FILE = "staff.txt";

class AirportExceptions : public exception{
	
	public:
		string error;
		
		AirportExceptions(string errorp): exception(){
				 error = string("AIRPORT EXCEPTION  ==>")+ " "+ errorp;
			
		}
		const char* what() const noexcept override{
		
			return error.c_str();
		}
};


class AirportUser {
public:
    string name;
public:
    AirportUser() {}
    AirportUser(string namep) { 
		name = namep;
	 }
    virtual void display() const = 0;
    string getname() const {
		return name; 
	 }
    friend class AirportSystem;
};


class Passenger : public AirportUser {
private:
    string passportNumber, seatNumber, mealPreference, feedback;
    Passenger() {}
    Passenger(string n, string p, string s, string m, string f = "No feedback")
        : AirportUser(n), passportNumber(p), seatNumber(s), mealPreference(m), feedback(f) {}
        
    void display() const {
        cout << "Passenger: " << name << " | Passport: " << passportNumber
             << " | Seat: " << seatNumber << " | Meal: " << mealPreference
             << " | Feedback: " << feedback << endl;
    }
    friend class AirportSystem;
};



class Cargo {
private:
    string cargoID, flightNumber, description;
    double weight;
    Cargo() {}
    Cargo(string id, string fnum, double w, string desc)
        : cargoID(id), flightNumber(fnum), weight(w), description(desc) {}
    void display() const {
        cout << "Cargo ID: " << cargoID << " | Flight: " << flightNumber
             << " | Weight: " << weight << " kg | Description: " << description << endl;
    }
    friend class AirportSystem;
};



class Staff : public AirportUser {
private:
    string role;
    double salary;
    Staff() {}
    Staff(string n, string r, double s) : AirportUser(n), role(r), salary(s) {}
    void display() const {
        cout << "Staff: " << name << " | Role: " << role << " | Salary: $" << salary << endl;
    }
    friend class AirportSystem;
};



class Flight {
public:
    string flightNumber, destination, departureTime, newDepartureTime;
    int totalSeats, bookedSeats, durationMinutes;
    bool delayed;
    vector<Passenger> passengers; // composition
    vector<Cargo> cargoList;

    Flight() {}
    Flight(string f, string d, string t, int s, int dur)
        : flightNumber(f), destination(d), departureTime(t), newDepartureTime(t),
          totalSeats(s), bookedSeats(0), durationMinutes(dur), delayed(false) {}

    void display() const {
        cout << "Flight " << flightNumber << " to " << destination << " at " << departureTime;
        if (delayed) cout << " (Delayed to " << newDepartureTime << ")";
        cout << " | Duration: " << durationMinutes << " mins | Seats: "
             << bookedSeats << "/" << totalSeats << endl;
    }
    
    bool operator==(const Flight &other) const {	
    	return (destination == other.destination && durationMinutes == other.durationMinutes&& flightNumber == other.flightNumber);
	}

friend class AirportSystem;

};



class AirportSystem {
public:
    vector<Flight> flights;
    vector<Staff> staffList;
    vector<string> destinationNames;
    vector<int> destinationCounts;

    AirportSystem() { 
		loadFromFiles(); 
	}
    ~AirportSystem() { 
		saveAllToFiles(); 
	}

    void loadFromFiles();
    void saveAllToFiles();
    void addFlight();
    void addStaff();
    void viewStaff();
    void delayFlight();
    void addPassengerToFlight();
    void addCargoToFlight();
    void viewPassengersOfFlight();
    void cancelPassengerFromFlight();
    void leaveFeedback();
    void viewAllFeedback();
    void compareFlights();
    void analyticsReport();
    void viewFlightDetails();
};
// Function Definitions (continued from class declaration)


void AirportSystem::loadFromFiles() {
    ifstream fin(FLIGHTS_FILE);
    
    if (!fin.is_open()){
    	string error = "Flight.txt Failed To Open While LOADING procces...";
    	throw AirportExceptions(error);
	}

    if (fin) {
        string f, d, t; int s, dur;
        while (fin >> f >> d >> t >> s >> dur) {
            flights.push_back(Flight(f, d, t, s, dur));
            int found = -1;
            for (int i = 0; i < destinationNames.size(); ++i) {
                if (destinationNames[i] == d) {
                    destinationCounts[i]++;
                    found = i;
                    break;
                }
            }
            if (found == -1) {
                destinationNames.push_back(d);
                destinationCounts.push_back(1);
            }
        }
        fin.close();
    }
    fin.open(PASSENGERS_FILE);
    if (!fin.is_open()){
    	string error = "Passenger.txt Failed To Open While LOADING procces...";
    	throw AirportExceptions(error);
	}
    if (fin) {
        string fnum, name, pass, seat, meal, fb;
        while (fin >> fnum >> name >> pass >> seat >> meal >> fb) {
            for (int i = 0; i < flights.size(); ++i) {
                if (flights[i].flightNumber == fnum) {
                    flights[i].passengers.push_back(Passenger(name, pass, seat, meal, fb));
                    flights[i].bookedSeats++;
                    break;
                }
            }
        }
        fin.close();
    }
    fin.open(CARGO_FILE);
    if (!fin.is_open()){
    	string error = "Cargo.txt Failed To Open While LOADING procces...";
    	throw AirportExceptions(error);
	}
    if (fin) {
        string cid, fnum, desc; double w;
        while (fin >> cid >> fnum >> w >> desc) {
            for (int i = 0; i < flights.size(); ++i) {
                if (flights[i].flightNumber == fnum) {
                    flights[i].cargoList.push_back(Cargo(cid, fnum, w, desc));
                    break;
                }
            }
        }
        fin.close();
    }
    fin.open(STAFF_FILE);
    if (!fin.is_open()){
    	string error = "Staff.txt Failed To Open While LOADING procces...";
    	throw AirportExceptions(error);
	}
    if (fin) {
        string name, role; double salary;
        while (fin >> name >> role >> salary) {
            staffList.push_back(Staff(name, role, salary));
        }
        fin.close();
    }
}

void AirportSystem::saveAllToFiles() {
    ofstream fout(FLIGHTS_FILE);
    if (!fout.is_open()){
    	string error = "Flight.txt Failed To Open While SAVING procces...";
    	throw AirportExceptions(error);
	}
    for (int i = 0; i < flights.size(); ++i)
        fout << flights[i].flightNumber << " " << flights[i].destination << " "
             << flights[i].departureTime << " " << flights[i].totalSeats << " " << flights[i].durationMinutes << "\n";
    fout.close();

    fout.open(PASSENGERS_FILE);
    if (!fout.is_open()){
    	string error = "Passenger.txt Failed To Open While SAVING procces...";
    	throw AirportExceptions(error);
	}
    for (int i = 0; i < flights.size(); ++i) {
        for (int j = 0; j < flights[i].passengers.size(); ++j) {
            Passenger &p = flights[i].passengers[j];
            fout << flights[i].flightNumber << " " << p.getname() << " " << p.passportNumber << " "
                 << p.seatNumber << " " << p.mealPreference << " " << p.feedback << "\n";
        }
    }
    fout.close();

    fout.open(CARGO_FILE);
    if (!fout.is_open()){
    	string error = "Cargo.txt Failed To Open While SAVING procces...";
    	throw AirportExceptions(error);
	}
    for (int i = 0; i < flights.size(); ++i) {
        for (int j = 0; j < flights[i].cargoList.size(); ++j) {
            Cargo &c = flights[i].cargoList[j];
            fout << c.cargoID << " " << c.flightNumber << " " << c.weight << " " << c.description << "\n";
        }
    }
    fout.close();

    fout.open(STAFF_FILE);
    if (!fout.is_open()){
    	string error = "Staff.txt Failed To Open While SAVING procces...";
    	throw AirportExceptions(error);
	}
    for (int i = 0; i < staffList.size(); ++i)
        fout << staffList[i].getname() << " " << staffList[i].role << " " << staffList[i].salary << "\n";
    fout.close();
}

void AirportSystem::addFlight() {
    string f, d, t; int s, dur;
    cout << "Flight Number: "; 
	cin >> f;
    cout << "Destination: "; 
	cin >> d;
    cout << "Departure Time: ";
	cin >> t;
    cout << "Seats: ";
	cin >> s;
    cout << "Duration (mins): ";
	cin >> dur;
    flights.push_back(Flight(f, d, t, s, dur));

    int found = -1;
    for (int i = 0; i < destinationNames.size(); ++i) {
        if (destinationNames[i] == d) {
            destinationCounts[i]++;
            found = i;
            break;
        }
    }
    if (found == -1) {
        destinationNames.push_back(d);
        destinationCounts.push_back(1);
    }
    cout << "Flight added!\n";
}

void AirportSystem::addStaff() {
    string name, role; double salary;
    cout << "Staff Name: "; 
	cin >> name;
    cout << "Role: "; 
	cin >> role;
    cout << "Salary: "; 
	cin >> salary;
    staffList.push_back(Staff(name, role, salary));
    cout << "Staff added!\n";
}

void AirportSystem::viewStaff() {
    for (int i = 0; i < staffList.size(); ++i)
        staffList[i].display();
}

void AirportSystem::delayFlight() {
    string fNum, newTime;
    cout << "Flight Number: "; 
	cin >> fNum;
    cout << "New Departure Time: "; 
	cin >> newTime;
    for (int i = 0; i < flights.size(); ++i) {
        if (flights[i].flightNumber == fNum) {
            flights[i].delayed = true;
            flights[i].newDepartureTime = newTime;
            cout << "Flight delayed to " << newTime << "\n";
            return;
        }
    }
    cout << "Flight not found.\n";
}

// Function Definitions (continued from class declaration)

// ... existing functions ...

void AirportSystem::addPassengerToFlight() {
    string fNum, name, passport, seat, meal;
    cout << "Flight Number: "; 
	cin >> fNum;
    cout << "Passenger Name: "; 
	cin.ignore();
    getline(cin, name);
    cout << "Passport Number: "; 
	cin >> passport;
    cout << "Seat Number: "; 
	cin >> seat;
    cout << "Meal Preference: "; 
	cin >> meal;
    for (int i = 0; i < flights.size(); ++i) {
        if (flights[i].flightNumber == fNum) {
            if (flights[i].bookedSeats >= flights[i].totalSeats) {
                cout << "Flight is fully booked.\n";
                return;
            }
            flights[i].passengers.push_back(Passenger(name, passport, seat, meal));
            flights[i].bookedSeats++;
            cout << "Passenger booked!\n";
            return;
        }
    }
    cout << "Flight not found.\n";
}

void AirportSystem::addCargoToFlight() {
    string fNum, cid, desc; double w;
    cout << "Flight Number: "; 
	cin >> fNum;
    cout << "Cargo ID: "; 
	cin >> cid;
    cout << "Weight (kg): ";
	 cin >> w;
    cout << "Description: ";
	 cin.ignore();
	  getline(cin, desc);
    for (int i = 0; i < flights.size(); ++i) {
        if (flights[i].flightNumber == fNum) {
            flights[i].cargoList.push_back(Cargo(cid, fNum, w, desc));
            cout << "Cargo added.\n";
            return;
        }
    }
    cout << "Flight not found.\n";
}

void AirportSystem::viewPassengersOfFlight() {
    string fNum;
    cout << "Flight Number: ";
	 cin >> fNum;
    for (int i = 0; i < flights.size(); ++i) {
        if (flights[i].flightNumber == fNum) {
            for (int j = 0; j < flights[i].passengers.size(); ++j) {
                flights[i].passengers[j].display();
            }
            return;
        }
    }
    cout << "Flight not found.\n";
}
// Function Definitions (continued from class declaration)

// ... previous functions ...

void AirportSystem::cancelPassengerFromFlight() {
    string fNum, passport;
    cout << "Flight Number: ";
	 cin >> fNum;
    cout << "Passport Number: "; 
	cin >> passport;
    for (int i = 0; i < flights.size(); ++i) {
        if (flights[i].flightNumber == fNum) {
            for (int j = 0; j < flights[i].passengers.size(); ++j) {
                if (flights[i].passengers[j].passportNumber == passport) {
                    flights[i].passengers.erase(flights[i].passengers.begin() + j);
                    flights[i].bookedSeats--;
                    cout << "Passenger canceled.\n";
                    return;
                }
            }
            cout << "Passenger not found.\n";
            return;
        }
    }
    cout << "Flight not found.\n";
}

void AirportSystem::leaveFeedback() {
    string fNum, passport, fb;
    cout << "Flight Number: "; 
	cin >> fNum;
    cout << "Passenger Passport: ";
	 cin >> passport;
    cout << "Feedback: "; 
	cin.ignore();
	 getline(cin, fb);
    for (int i = 0; i < flights.size(); ++i) {
        if (flights[i].flightNumber == fNum) {
            for (int j = 0; j < flights[i].passengers.size(); ++j) {
                if (flights[i].passengers[j].passportNumber == passport) {
                    flights[i].passengers[j].feedback = fb;
                    cout << "Feedback recorded.\n";
                    return;
                }
            }
        }
    }
    cout << "Passenger or flight not found.\n";
}

void AirportSystem::viewAllFeedback() {
    for (int i = 0; i < flights.size(); ++i) {
        cout << "Feedbacks for Flight " << flights[i].flightNumber << ":\n";
        for (int j = 0; j < flights[i].passengers.size(); ++j) {
            cout << "- " << flights[i].passengers[j].passportNumber << ": "
                 << flights[i].passengers[j].feedback << endl;
        }
    }
}

void AirportSystem::compareFlights() {
    string f1, f2;
    cout << "First Flight Number: "; 
	cin >> f1;
    cout << "Second Flight Number: "; 
	cin >> f2;
    Flight *flight1 = nullptr, *flight2 = nullptr;

    for (auto &f : flights) {
        if (f.flightNumber == f1) {
			flight1 = &f;
		}
		
        if (f.flightNumber == f2) {
        	flight2 = &f;	
		}
		
    }

    if (flight1 && flight2) {
        if (*flight1 == *flight2)
            cout << "Flights have same destination and duration.\n";
        else if (flight1->destination == flight2->destination)
            cout << "Same destination, different durations.\n";
        else
            cout << "Flights are different.\n";
    } else {
        cout << "One or both flights not found.\n";
    }
}


void AirportSystem::analyticsReport() {
    cout << "\n=== Analytics Report ===\n";
    string topDest = "";
    int maxCount = 0;
    for (int i = 0; i < destinationNames.size(); ++i) {
        cout << destinationNames[i] << ": " << destinationCounts[i] << " flights\n";
        if (destinationCounts[i] > maxCount) {
            maxCount = destinationCounts[i];
            topDest = destinationNames[i];
        }
    }
    if (topDest != "") {
        cout << "Most Popular Destination: " << topDest << " (" << maxCount << " flights)\n";
    }
}

void AirportSystem::viewFlightDetails() {
    string fNum;
    int found = 0;
    cout << "Flight Number: "; 
	cin >> fNum;
    for (int i = 0; i < flights.size(); ++i) {
        if (flights[i].flightNumber == fNum) {
            flights[i].display();
            found = 1;
        }
    }
    if (!found) {
        cout << "Flight not found.\n";
    }
}
class UserAuth {
    string adminUser = "admin";
    string adminPass = "1234";
public:
    bool login() {
        string user, pass;
        cout << "Username: "; 
		cin >> user;
        cout << "Password: ";
		 cin >> pass;
        return user == adminUser && pass == adminPass;
    }
};
// Main Function
int main() {
	system("color 0B"); // 0 = Black background, A = Light green text
	cout << setw(60) << "**************************************************" << endl;
    cout << setw(60) << "*     WELCOME TO AIRPORT MANAGEMENT SYSTEM       *" << endl;
    cout << setw(60) << "*                 DEVELOPED BY:                  *" << endl;
    cout << setw(60) << "*   1) Mutahir Ahmed   24K-0030                  *" << endl;
    cout << setw(60) << "*   2) Burair  Hyder   24K-0804                  *" << endl;
    cout << setw(60) << "*   3) Sameed Imran    24K-1036                  *" << endl;
    cout << setw(60) << "*   4) Ammar  Kamran   24K-0732                  *" << endl;
    cout << setw(60) << "*                                                *" << endl;
    cout << setw(60) << "**************************************************" << endl;
     UserAuth auth;

    if (!auth.login()) {
        cout << "Access denied.\n";
        return 1;
    }
try{
    AirportSystem sys;

    int choice;
    
    do {
        cout << "\n==== Airport Main Menu ====\n";
        cout << "1.  Add Flight\n";
        cout << "2.  Add Passenger to Flight\n";
        cout << "3.  Add Cargo to Flight\n";
        cout << "4.  Add Staff Member\n";
        cout << "5.  View Staff\n";
        cout << "6.  Delay Flight\n";
        cout << "7.  View Passengers of Flight\n";
        cout << "8.  Cancel Passenger\n";
        cout << "9.  Compare Flights\n";
        cout << "10. Leave Feedback\n";
        cout << "11. View All Feedback\n";
        cout << "12. Analytics Report\n";
        cout << "13. View Flight Details\n";
        cout << "0.  Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1: sys.addFlight(); 
					break;
            case 2: sys.addPassengerToFlight(); 
					break;
            case 3: sys.addCargoToFlight(); 
					break;
            case 4: sys.addStaff();
					break;
            case 5: sys.viewStaff(); 
					break;
            case 6: sys.delayFlight(); 
					break;
            case 7: sys.viewPassengersOfFlight(); 
					break;
            case 8: sys.cancelPassengerFromFlight(); 
					break;
            case 9: sys.compareFlights(); 
					break;
            case 10: sys.leaveFeedback(); 
					break;
            case 11: sys.viewAllFeedback(); 
					break;
            case 12: sys.analyticsReport(); 
					break;
            case 13: sys.viewFlightDetails(); 
					break;
            case 0: cout << "ThankYou for Flying with US!\n"; 
					break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    }
catch(exception &e){
	cout <<e.what()<<endl;
}

    return 0;
}
