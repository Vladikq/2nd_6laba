#pragma once

#include "../DatabaseManager.hpp"
#include "../repositories/CategoryService.hpp"
#include "../repositories/SupplierService.hpp"
#include "../repositories/ProductService.hpp"
#include "../repositories/CustomerService.hpp"
#include "../repositories/OrderService.hpp"
#include "../repositories/AnalyticsService.hpp"
#include <string>
#include <map>

class UserInterface {
public:
    UserInterface(DatabaseManager& db);
    void run(int argc, char* argv[]);

private:

    // Категории
    void handleAddCategory(const std::string& name);
    void handleDeleteCategory(const std::string& name);
    void handleListCategories();

    // Поставщики
    void handleAddSupplier(const std::string& name, const std::string& email);
    void handleDeleteSupplier(const std::string& email);
    void handleListSuppliers();

    // Товары
    void handleAddProduct(const std::map<std::string, std::string>& args);
    void handleListProducts();

    // Клиенты
    void handleAddCustomer(const std::string& full_name, const std::string& email);
    void handleListCustomers();

    // Заказы
    void handleCreateOrder(const std::map<std::string, std::string>& args);
    void handleListOrders(const std::map<std::string, std::string>& args);

    // Аналитика
    void handleTopProducts(int limit);  // ТОП-N товаров по категориям
    void handleUnusedSuppliers();       // Поставщики, чьи товары никто не покупал
    void handleSpendingReport();        // Отчёт по тратам клиентов
    void handleExpensiveProducts();     // Товары дороже среднего в своей категории
    void handleSharedPurchases();       // Клиенты, купившие один и тот же товар 
    void handleInactiveCustomers();     // Клиенты без заказов
    void handleAvgCheck();              // Средний чек по клиентам
    void handlePopularProducts();       // Товары, купленные более одного раза
    void handleRevenueByCategory();     // Выручка по категориям
    void handleSupplierSales();         // Продажи по поставщикам

    void printUsage();

    CategoryService categoryService;
    SupplierService supplierService;
    ProductService productService;
    CustomerService customerService;
    OrderService orderService;
    AnalyticsService analyticsService;
};
