#pragma once
#include "../DatabaseManager.hpp"
#include "../DataModels.hpp"
#include <vector>
#include <string>

class AnalyticsService {
public:
    AnalyticsService(DatabaseManager& db);
    
    std::vector<TopProduct> getTopProductsPerCategory(int limit);
    std::vector<UnusedSupplier> getUnusedSuppliers();
    std::vector<CustomerSpending> getCustomerSpendingReport();
    std::vector<ExpensiveProduct> getProductsAboveCategoryAvg();
    std::vector<SharedPurchase> getSharedPurchases();
    std::vector<InactiveCustomer> getInactiveCustomers();
    std::vector<AvgCheck> getAvgCheckPerCustomer();
    std::vector<PopularProduct> getPopularProducts();
    std::vector<RevenueByCategory> getRevenueByCategory();
    std::vector<SupplierSales> getSupplierSales();

private:
    DatabaseManager& db;
};
