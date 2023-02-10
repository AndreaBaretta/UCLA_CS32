#include <iostream>
#include <string>
using namespace std;

// Your declarations and implementations would go here

class Vehicle {
  private:
    const string m_id;

  public:
    Vehicle(const string id) : m_id(id) {}
    string id() const { return m_id.substr(0, m_id.length() - 1); }
    virtual string description() const = 0;
    bool canHover() const {
        return m_id[m_id.length() - 1] == '1';
    }
    virtual ~Vehicle() {}
};

class Drone : public Vehicle {
  public:
    Drone(const string id) : Vehicle(id + "1") {}
    string description() const {
        return "a drone";
    }
    ~Drone() {
        cout << "Destroying " << id() << ", a drone" << endl;
    }
};

class Satellite : public Vehicle {
  public:
    Satellite(const string id) : Vehicle(id + "0") {}
    string description() const {
        return "a satellite";
    }
    ~Satellite() {
        cout << "Destroying the satellite " << id() << endl;
    }
};

class Balloon : public Vehicle {
  private:
    double m_diameter;
  public:
    Balloon(const string id, const double diameter) : Vehicle(id + "1"), m_diameter(diameter) {}
    string description() const {
        return "a " + string(m_diameter >= 8 ? "large" : "small") + " balloon";
    }
    ~Balloon() {
        cout << "Destroying the balloon " << id() << endl;
    }
};

void display(const Vehicle* v) {
    cout << v->id() << " is " << v->description();
    if (v->canHover()) {
        cout << ", so it can hover";
    }
    cout << endl;
}

int main() {
    Vehicle* fleet[4];
    fleet[0] = new Drone("CB4UP");
      // Balloons have an id and a diameter in meters  Balloons with a
      // diameter under 8 meters are small balloons; those with a diameter
      // 8 meters or more are large balloons
    fleet[1] = new Balloon("99LB", 6.3);
    fleet[2] = new Balloon("CN20230201", 30.2);
    fleet[3] = new Satellite("EB8675309");

    for (int k = 0; k < 4; k++) {
        display(fleet[k]);
    }

      // Clean up the vehicles before exiting
    cout << "Cleaning up" << endl;
    for (int k = 0; k < 4; k++) {
        delete fleet[k];
    }
}

// CB4UP is a drone, so it can hover
// 99LB is a small balloon, so it can hover
// CN20230201 is a large balloon, so it can hover
// EB8675309 is a satellite
// Cleaning up
// Destroying CB4UP, a drone
// Destroying the balloon 99LB
// Destroying the balloon CN20230201
// Destroying the satellite EB8675309