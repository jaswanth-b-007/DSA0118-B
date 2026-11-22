#include <iostream>
#include <memory>
#include <string>
#include <vector>
class Vehicle {
public:
    Vehicle(std::string reg, double rate) : reg_(std::move(reg)), rate_(rate) {}
    virtual ~Vehicle() = default;
    virtual double charge(int days) const = 0;
    virtual std::string type() const = 0;
    const std::string& reg() const { return reg_; }
protected:
    double rate() const { return rate_; }
private:
    std::string reg_;
    double rate_;
};
class Bicycle : public Vehicle {
public:
    Bicycle(std::string reg, double rate) : Vehicle(std::move(reg), rate) {}
    double charge(int days) const override { return rate() * days; }
    std::string type() const override { return "Bicycle"; }
};
class TwoWheeler : public Vehicle {
    double fuel_;
public:
    TwoWheeler(std::string reg, double rate, double fuel) : Vehicle(std::move(reg), rate), fuel_(fuel) {}
    double charge(int days) const override { return rate() * days + 50.0; }
    std::string type() const override { return "TwoWheeler"; }
};
class Car : public Vehicle {
    int seats_;
public:
    Car(std::string reg, double rate, int seats) : Vehicle(std::move(reg), rate), seats_(seats) {}
    double charge(int days) const override { return rate() * days + 200.0; }
    std::string type() const override { return "Car"; }
};
class ElectricCar : public Car {
    double kwh_;
public:
    ElectricCar(std::string reg, double rate, int seats, double kwh)
        : Car(std::move(reg), rate, seats), kwh_(kwh) {}
    double charge(int days) const override { return Car::charge(days) + kwh_ * 10.0; }
    std::string type() const override { return "ElectricCar"; }
};
int main()
{
    std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<Bicycle>("TN01 AB 1001", 20.0));
    fleet.push_back(std::make_unique<TwoWheeler>("TN02 CD 2002", 40.0, 5.0));
    fleet.push_back(std::make_unique<Car>("TN03 EF 3003", 100.0, 5));
    fleet.push_back(std::make_unique<ElectricCar>("TN04 GH 4004", 80.0, 4, 15.0));
    for (const auto& v : fleet) {
        std::cout << v->type() << " " << v->reg() << " 3 days = " << v->charge(3) << '\n';
    }
}