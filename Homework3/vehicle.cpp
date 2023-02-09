#include <iostream>
#include <string>
using namespace std;

// Your declarations and implementations would go here

class Vehicle {
  private:
    const string id;

  public:
    Vehicle(const string id) : id(id) {}
    virtual string id() const { return id; }
    virtual string description() const;
    virtual bool canHover() const;
};

void display(const Vehicle* v) {
    cout << v->id() << " is " << v->description();
    if (v->canHover())
        cout << ", so it can hover";
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

    for (int k = 0; k < 4; k++)
        display(fleet[k]);

      // Clean up the vehicles before exiting
    cout << "Cleaning up" << endl;
    for (int k = 0; k < 4; k++)
        delete fleet[k];
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