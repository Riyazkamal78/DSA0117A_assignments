#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

class Vehicle {
    std::string registration_;
    double dailyRate_;
    bool rented_ = false;

protected:
    double dailyRate() const { return dailyRate_; }

public:
    Vehicle(std::string registration, double dailyRate)
        : registration_(std::move(registration)), dailyRate_(dailyRate) {}
    virtual ~Vehicle() = default;

    const std::string& registration() const { return registration_; }
    bool isRented() const { return rented_; }
    void markRented() { rented_ = true; }
    void markAvailable() { rented_ = false; }

    virtual std::string_view type() const = 0;
    virtual std::string_view unit() const { return "none"; }
    virtual double charge(int days, double used) const = 0;
    virtual void details(std::ostream& output) const {
        output << " | INR " << dailyRate_ << "/day";
    }
};

struct ServiceInfo {
    std::string dueDate;
};

class MotorVehicle : public Vehicle {
    double levelPercent_;
    ServiceInfo service_;

protected:
    MotorVehicle(std::string registration, double dailyRate, double level,
                 ServiceInfo service)
        : Vehicle(std::move(registration), dailyRate), levelPercent_(level),
          service_(std::move(service)) {}

public:
    void details(std::ostream& output) const override {
        Vehicle::details(output);
        output << " | level " << levelPercent_ << "% | service " << service_.dueDate;
    }
};

class Bicycle final : public Vehicle {
public:
    using Vehicle::Vehicle;
    std::string_view type() const override { return "Bicycle"; }
    double charge(int days, double) const override { return days * dailyRate(); }
};

class TwoWheeler final : public MotorVehicle {
public:
    TwoWheeler(std::string registration, double dailyRate, double level, ServiceInfo service)
        : MotorVehicle(std::move(registration), dailyRate, level, std::move(service)) {}
    std::string_view type() const override { return "Two-wheeler"; }
    std::string_view unit() const override { return "L"; }
    double charge(int days, double litres) const override {
        return days * dailyRate() + 90 * litres;
    }
};

class Car : public MotorVehicle {
    int seats_;

public:
    Car(std::string registration, double dailyRate, double level,
        ServiceInfo service, int seats)
        : MotorVehicle(std::move(registration), dailyRate, level, std::move(service)),
          seats_(seats) {}
    std::string_view type() const override { return "Car"; }
    std::string_view unit() const override { return "L"; }
    double charge(int days, double litres) const override {
        return days * dailyRate() + 105 * litres;
    }
    void details(std::ostream& output) const override {
        MotorVehicle::details(output);
        output << " | " << seats_ << " seats";
    }
};

class ElectricCar final : public Car {
    double batteryKWh_;

public:
    ElectricCar(std::string registration, double dailyRate, double level,
                ServiceInfo service, int seats, double battery)
        : Car(std::move(registration), dailyRate, level, std::move(service), seats),
          batteryKWh_(battery) {}
    std::string_view type() const override { return "Electric car"; }
    std::string_view unit() const override { return "kWh"; }
    double charge(int days, double kWh) const override {
        return days * dailyRate() + 12 * kWh;
    }
    void details(std::ostream& output) const override {
        Car::details(output);
        output << " | " << batteryKWh_ << " kWh battery";
    }
};

// Extension: the original vehicle classes and Fleet require no edits.
class ElectricBus final : public MotorVehicle {
    double batteryKWh_;

public:
    ElectricBus(std::string registration, double dailyRate, double level,
                ServiceInfo service, double battery)
        : MotorVehicle(std::move(registration), dailyRate, level, std::move(service)),
          batteryKWh_(battery) {}
    std::string_view type() const override { return "Electric bus"; }
    std::string_view unit() const override { return "kWh"; }
    double charge(int days, double kWh) const override {
        return days * dailyRate() + 15 * kWh;
    }
    void details(std::ostream& output) const override {
        MotorVehicle::details(output);
        output << " | " << batteryKWh_ << " kWh battery";
    }
};

class Fleet {
    static constexpr double lateFee = 150;
    std::array<Vehicle*, 8> vehicles_{};
    std::size_t count_ = 0;

    Vehicle* find(std::string_view registration) const {
        for (std::size_t i = 0; i < count_; ++i) {
            if (vehicles_[i]->registration() == registration) {
                return vehicles_[i];
            }
        }
        return nullptr;
    }

public:
    bool add(Vehicle& vehicle) {
        if (count_ == vehicles_.size() || find(vehicle.registration())) {
            return false;
        }
        vehicles_[count_++] = &vehicle;
        return true;
    }

    bool rent(std::string_view registration) {
        Vehicle* vehicle = find(registration);
        if (vehicle == nullptr || vehicle->isRented()) {
            std::cout << "Rent rejected: " << registration << '\n';
            return false;
        }
        vehicle->markRented();
        std::cout << "Rented " << registration << " (" << vehicle->type() << ")\n";
        return true;
    }

    bool returnVehicle(std::string_view registration, int days, int lateDays, double used) {
        Vehicle* vehicle = find(registration);
        if (vehicle == nullptr || !vehicle->isRented() || days <= 0 ||
            lateDays < 0 || lateDays > days || !(used >= 0) ||
            (vehicle->unit() == "none" && used != 0)) {
            std::cout << "Return rejected: " << registration << '\n';
            return false;
        }

        const double fee = lateDays * lateFee;
        const double total = vehicle->charge(days, used) + fee;
        vehicle->markAvailable();
        std::cout << "Returned " << registration << ": INR " << total
                  << " (late fee " << fee << ")\n";
        return true;
    }

    void list() const {
        for (std::size_t i = 0; i < count_; ++i) {
            const Vehicle& vehicle = *vehicles_[i];
            std::cout << vehicle.registration() << " | " << vehicle.type()
                      << " | " << (vehicle.isRented() ? "Rented" : "Available");
            vehicle.details(std::cout);
            std::cout << '\n';
        }
    }
};

int main() {
    Bicycle bicycle("B01", 150);
    TwoWheeler scooter("T01", 400, 80, {"2026-12-01"});
    Car car("C01", 1200, 70, {"2026-12-07"}, 5);
    ElectricCar electric("E01", 1500, 90, {"2026-12-10"}, 5, 50);
    ElectricBus bus("EB01", 4000, 95, {"2026-12-15"}, 180);
    Fleet fleet;
    for (Vehicle* vehicle : std::array<Vehicle*, 4>{&bicycle, &scooter, &car, &electric}) {
        if (!fleet.add(*vehicle) || !fleet.rent(vehicle->registration())) {
            return 1;
        }
    }

    std::cout << std::fixed << std::setprecision(2);
    fleet.rent("C01");
    fleet.returnVehicle("B01", 2, 0, 0);
    fleet.returnVehicle("T01", 2, 0, 3);
    fleet.returnVehicle("C01", 3, 1, 5);
    fleet.returnVehicle("E01", 2, 0, 18);

    std::cout << "\nExtension\n";
    if (!fleet.add(bus) || !fleet.rent("EB01")) {
        return 1;
    }
    fleet.returnVehicle("EB01", 1, 0, 30);
    std::cout << "\nFleet status\n";
    fleet.list();
}
