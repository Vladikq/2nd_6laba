#pragma once
#include <string>

struct Product {
    int id;
    std::string name;
    int price;
    std::string category_name;
    std::string supplier_email;
};

struct Customer {
    int id;
    std::string full_name;
    std::string email;
};

struct Order {
    int id;
    std::string customer_email;
    std::string product_name;
    std::string order_date;
    std::string status;
    std::string name;   // alias
    std::string email;  // alias
};

struct Category {
    int id;
    std::string name;
};

struct Supplier {
    int id;
    std::string name;
    std::string email;
};

// Analytics structures
struct TopProduct {
    std::string name;
    std::string category_name;
    int order_count;
    int row_number;
    std::string category;  // alias
    std::string product;   // alias
    int price;            // alias
};

struct UnusedSupplier {
    std::string name;
    std::string email;
};

struct CustomerSpending {
    std::string full_name;
    std::string email;
    int total_spent;
};

struct ExpensiveProduct {
    std::string name;
    int price;
    std::string category_name;
    double category_avg;
    std::string product;   // alias
    std::string category;  // alias
};

struct SharedPurchase {
    std::string customer1_email;
    std::string customer2_email;
    std::string product_name;
    std::string customer1;  // alias
    std::string customer2;  // alias
    std::string product;    // alias
};

struct InactiveCustomer {
    std::string full_name;
    std::string email;
};

struct AvgCheck {
    std::string customer_email;
    double avg_check;
    std::string customer;  // alias
    double avg_spent;      // alias
};

struct PopularProduct {
    std::string name;
    int purchase_count;
};

struct RevenueByCategory {
    std::string category_name;
    int total_revenue;
    std::string category;  // alias
};

struct SupplierSales {
    std::string supplier_name;
    int total_sales;
    std::string supplier;  // alias
};
