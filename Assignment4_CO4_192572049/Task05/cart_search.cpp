#include <iomanip>
#include <iostream>
#include <string>

class Cart {
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0;              // percent

public:
    Cart& add(const std::string& item, double price)
    {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    Cart& setDiscount(double discount)
    {
        this->discount = discount;
        return *this;
    }

    double total() const
    {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }
};

const Cart* highestCart(Cart* const* carts, std::size_t count) {
    const Cart* best = nullptr;
    double bestTotal = 0;
    for (std::size_t i = 0; i < count; ++i) {
        const Cart* cart = carts[i];
        if (cart == nullptr) {
            continue;
        }
        const double total = cart->total();
        if (best == nullptr || total > bestTotal) {
            best = cart;
            bestTotal = total;
        }
    }
    return best;
}

int main() {
    Cart first;
    first.add("Rice", 60).add("Dal", 120);

    Cart* pointer = &first;
    pointer->add("Oil", 180);

    Cart& reference = first;
    reference.setDiscount(10);

    Cart second;
    second.add("Milk", 80).add("Bread", 55);

    Cart* dynamic = new Cart;
    dynamic->add("Groceries", 450).setDiscount(5);

    Cart* active[] = {&first, nullptr, &second, dynamic};
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Named / pointer / reference cart: " << first.total() << '\n';
    std::cout << "Second cart: " << second.total() << '\n';
    std::cout << "Dynamically allocated cart: " << dynamic->total() << '\n';
    if (const Cart* best = highestCart(active, sizeof(active) / sizeof(active[0]))) {
        std::cout << "Highest active cart total: " << best->total() << '\n';
    }

    delete dynamic;
    active[3] = nullptr;
}
