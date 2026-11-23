#pragma once
#include "billing.hpp"
#include <array>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

inline void check_baseline(std::ostream& out)
{
    using namespace hospital;
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 38000, 2000));
    bills.push_back(std::make_unique<DayCareBill>("Asha", 18000));
    bills.push_back(std::make_unique<EmergencyBill>("Dev", 2000, 3, 500));

    SelfPay self;
    Insurance insurance(40000, 0.20);
    GovernmentScheme scheme;
    std::array<const Payer*, 3> payers{&self, &insurance, &scheme};
    const double totals[] = {1700, 47500, 18000, 3500};
    const double expected[][3] = {{0, 1360, 1000}, {0, 38000, 30000},
                                  {0, 14400, 15000}, {0, 2800, 2000}};

    out << std::fixed << std::setprecision(2);
    out << "Bill Gross | Self cover/due | Insurance cover/due | Scheme cover/due\n";
    double sum = 0;
    for (std::size_t i = 0; i < bills.size(); ++i) {
        const Bill& bill = *bills[i];
        double gross = bill.total();
        if (gross != totals[i]) throw std::logic_error("bill total mismatch");
        out << bill.type() << ' ' << gross;
        sum += gross;
        for (std::size_t j = 0; j < payers.size(); ++j) {
            double covered = payers[j]->amountCovered(bill);
            if (std::abs(covered - expected[i][j]) > 0.005)
                throw std::logic_error("payer amount mismatch");
            out << " | " << covered << '/' << gross - covered;
        }
        out << '\n';
    }
    if (sum != 70700) throw std::logic_error("daily total mismatch");
    out << "Verified 12 payer results. Daily total: " << sum << '\n';
}
