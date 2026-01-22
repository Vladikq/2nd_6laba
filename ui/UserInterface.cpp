#include "UserInterface.hpp"
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <iomanip>  // для setprecision

using namespace std;

// Глобальная вспомогательная функция (видна только в этом файле)
map<string, string> parseArgs(int start, int argc, char* argv[]) {
    map<string, string> args;
    for (int i = start; i < argc; ++i) {
        string arg = argv[i];
        if (arg.substr(0, 2) == "--") {
            string key = arg.substr(2);
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                args[key] = argv[i + 1];
                i++;
            } else {
                args[key] = "true";
            }
        }
    }
    return args;
}

UserInterface::UserInterface(DatabaseManager& db)
    : categoryService(db)
    , supplierService(db)
    , productService(db)
    , customerService(db)
    , orderService(db)
    , analyticsService(db) {}

void UserInterface::run(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return;
    }

    string cmd = argv[1];

    // Category
    if (cmd == "add-category" && argc == 3) {
        handleAddCategory(argv[2]);
    } else if (cmd == "delete-category" && argc == 3) {
        handleDeleteCategory(argv[2]);
    } else if (cmd == "list-categories" && argc == 2) {
        handleListCategories();

    // Supplier
    } else if (cmd == "add-supplier" && argc == 4) {
        handleAddSupplier(argv[2], argv[3]);
    } else if (cmd == "delete-supplier" && argc == 3) {
        handleDeleteSupplier(argv[2]);
    } else if (cmd == "list-suppliers" && argc == 2) {
        handleListSuppliers();

    // Product
    } else if (cmd == "add-product") {
        auto args = ::parseArgs(2, argc, argv);
        handleAddProduct(args);
    } else if (cmd == "list-products" && argc == 2) {
        handleListProducts();

    // Customer
    } else if (cmd == "add-customer" && argc == 4) {
        handleAddCustomer(argv[2], argv[3]);
    } else if (cmd == "list-customers" && argc == 2) {
        handleListCustomers();

    // Order
    } else if (cmd == "create-order") {
        auto args = ::parseArgs(2, argc, argv);
        handleCreateOrder(args);
    } else if (cmd == "list-orders") {
        auto args = ::parseArgs(2, argc, argv);
        handleListOrders(args);

    // Analytics
    } else if (cmd == "analytics" && argc >= 3) {
        string subcmd = argv[2];
        if (subcmd == "top-products" && argc == 4) {
            try {
                int limit = stoi(argv[3]);
                if (limit > 0) {
                    handleTopProducts(limit);
                } else {
                    cerr << "Лимит должен быть положительным\n";
                }
            } catch (...) {
                cerr << "Некорректное число: " << argv[3] << "\n";
            }
        } else if (subcmd == "unused-suppliers") {
            handleUnusedSuppliers();
        } else if (subcmd == "spending-report") {
            handleSpendingReport();
        } else if (subcmd == "expensive-products") {
            handleExpensiveProducts();
        } else if (subcmd == "shared-purchases") {
            handleSharedPurchases();
        } else if (subcmd == "inactive-customers") {
            handleInactiveCustomers();
        } else if (subcmd == "avg-check") {
            handleAvgCheck();
        } else if (subcmd == "popular-products") {
            handlePopularProducts();
        } else if (subcmd == "revenue-by-category") {
            handleRevenueByCategory();
        } else if (subcmd == "supplier-sales") {
            handleSupplierSales();
        } else {
            printUsage();
        }

    } else {
        printUsage();
    }
}

// --- Category ---
void UserInterface::handleAddCategory(const string& name) {
    if (categoryService.findByName(name).has_value()) {
        cout << "Категория уже существует: " << name << endl;
        return;
    }
    if (categoryService.add(name)) {
        cout << "Добавлена категория: " << name << endl;
    } else {
        cerr << "Ошибка при добавлении категории" << endl;
    }
}

void UserInterface::handleDeleteCategory(const string& name) {
    if (!categoryService.findByName(name).has_value()) {
        cout << "Категория не найдена: " << name << endl;
        return;
    }
    if (categoryService.removeByName(name)) {
        cout << "Удалена категория: " << name << endl;
    } else {
        cerr << "Ошибка при удалении категории" << endl;
    }
}

void UserInterface::handleListCategories() {
    auto categories = categoryService.findAll();
    if (categories.empty()) {
        cout << "Нет категорий" << endl;
    } else {
        cout << "Категории:\n";
        for (const auto& c : categories) {
            cout << "- " << c.name << '\n';
        }
    }
}

// --- Supplier ---
void UserInterface::handleAddSupplier(const string& name, const string& email) {
    if (supplierService.findByEmail(email).has_value()) {
        cout << "Поставщик с email уже существует: " << email << endl;
        return;
    }
    if (supplierService.add(name, email)) {
        cout << "Добавлен поставщик: " << name << " (" << email << ")" << endl;
    } else {
        cerr << "Ошибка при добавлении поставщика" << endl;
    }
}

void UserInterface::handleDeleteSupplier(const string& email) {
    auto sup = supplierService.findByEmail(email);
    if (!sup.has_value()) {
        cout << "Поставщик не найден по email: " << email << endl;
        return;
    }
    if (supplierService.removeByEmail(email)) {
        cout << "Удалён поставщик: " << sup->name << " (" << email << ")" << endl;
    } else {
        cerr << "Ошибка при удалении поставщика" << endl;
    }
}

void UserInterface::handleListSuppliers() {
    auto suppliers = supplierService.findAll();
    if (suppliers.empty()) {
        cout << "Нет поставщиков" << endl;
    } else {
        cout << "Поставщики:\n";
        for (const auto& s : suppliers) {
            cout << "- " << s.name << " <" << s.email << ">\n";
        }
    }
}

// --- Product ---
void UserInterface::handleAddProduct(const map<string, string>& args) {
    auto nameIt = args.find("name");
    auto priceIt = args.find("price");
    auto categoryIt = args.find("category");
    auto supplierIt = args.find("supplier");

    if (nameIt == args.end() || priceIt == args.end() ||
        categoryIt == args.end() || supplierIt == args.end()) {
        cerr << "Ошибка: нужны флаги --name, --price, --category, --supplier\n";
        cout << "Пример: add-product --name \"Телефон\" --price 30000 --category Электроника --supplier test@example.com\n";
        return;
    }

    string name = nameIt->second;
    string priceStr = priceIt->second;
    string category = categoryIt->second;
    string email = supplierIt->second;

    try {
        size_t pos;
        int price = stoi(priceStr, &pos);
        if (pos != priceStr.length() || price <= 0) {
            throw invalid_argument("bad number");
        }

        if (productService.add(name, price, name, email)) {
            cout << "Товар добавлен: " << name << " (" << price << " руб.)\n";
        }
    } catch (...) {
        cerr << "Ошибка: цена должна быть положительным целым числом\n";
    }
}

void UserInterface::handleListProducts() {
    auto products = productService.findAll();
    if (products.empty()) {
        cout << "Нет товаров" << endl;
    } else {
        cout << "Товары:\n";
        for (const auto& p : products) {
            cout << "- " << p.name
                 << " | " << p.price << " руб."
                 << " | Категория: " << p.name
                 << " | Поставщик: " << p.supplier_email << "\n";
        }
    }
}

// --- Customer ---
void UserInterface::handleAddCustomer(const string& full_name, const string& email) {
    if (customerService.findByEmail(email).has_value()) {
        cout << "Клиент с email уже существует: " << email << endl;
        return;
    }
    if (customerService.add(full_name, email)) {
        cout << "Добавлен клиент: " << full_name << " (" << email << ")" << endl;
    } else {
        cerr << "Ошибка при добавлении клиента" << endl;
    }
}

void UserInterface::handleListCustomers() {
    auto customers = customerService.findAll();
    if (customers.empty()) {
        cout << "Нет клиентов" << endl;
    } else {
        cout << "Клиенты:\n";
        for (const auto& c : customers) {
            cout << "- " << c.full_name << " <" << c.email << ">\n";
        }
    }
}

// --- Order ---
void UserInterface::handleCreateOrder(const map<string, string>& args) {
    auto custIt = args.find("customer");
    auto prodIt = args.find("product");

    if (custIt == args.end() || prodIt == args.end()) {
        cerr << "Ошибка: нужны --customer <email> и --product <название>\n";
        return;
    }

    if (orderService.create(custIt->second, prodIt->second)) {
        cout << "Заказ создан для " << custIt->second << " на товар: " << prodIt->second << endl;
    }
}

void UserInterface::handleListOrders(const map<string, string>& args) {
    auto custIt = args.find("customer");
    if (custIt != args.end()) {
        auto orders = orderService.listByCustomer(custIt->second);
        if (orders.empty()) {
            cout << "Нет заказов для клиента: " << custIt->second << endl;
        } else {
            cout << "Заказы клиента " << custIt->second << ":\n";
            for (const auto& o : orders) {
                cout << "- " << o.name
                     << " | " << o.order_date
                     << " | статус: " << o.status << "\n";
            }
        }
    } else {
        auto orders = orderService.listAll();
        if (orders.empty()) {
            cout << "Нет заказов" << endl;
        } else {
            cout << "Все заказы:\n";
            for (const auto& o : orders) {
                cout << "- " << o.email
                     << " → " << o.name
                     << " | " << o.order_date
                     << " | " << o.status << "\n";
            }
        }
    }
}

// --- Analytics (старые) ---
void UserInterface::handleTopProducts(int limit) {
    auto products = analyticsService.getTopProductsPerCategory(limit);
    if (products.empty()) {
        cout << "Нет данных\n";
    } else {
        cout << "ТОП-" << limit << " товаров по категориям:\n";
        string currentCategory = "";
        for (const auto& p : products) {
            if (p.category != currentCategory) {
                cout << "\nКатегория: " << p.category << "\n";
                currentCategory = p.category;
            }
            cout << "  - " << p.product << " (" << p.price << " руб.)\n";
        }
    }
}

void UserInterface::handleUnusedSuppliers() {
    auto suppliers = analyticsService.getUnusedSuppliers();
    if (suppliers.empty()) {
        cout << "Все поставщики используются\n";
    } else {
        cout << "Поставщики без продаж:\n";
        for (const auto& s : suppliers) {
            cout << "- " << s.name << " (" << s.email << ")\n";
        }
    }
}

void UserInterface::handleSpendingReport() {
    auto report = analyticsService.getCustomerSpendingReport();
    if (report.empty()) {
        cout << "Нет клиентов\n";
    } else {
        cout << "Отчёт по тратам клиентов:\n";
        for (const auto& r : report) {
            cout << "- " << r.full_name << " (" << r.email << ") — " << r.total_spent << " руб.\n";
        }
    }
}

void UserInterface::handleExpensiveProducts() {
    auto products = analyticsService.getProductsAboveCategoryAvg();
    if (products.empty()) {
        cout << "Нет товаров дороже среднего\n";
    } else {
        cout << "Товары дороже среднего по категории:\n";
        for (const auto& p : products) {
            cout << "- " << p.product << " (" << p.price << " руб.) в категории " << p.category << "\n";
        }
    }
}

void UserInterface::handleSharedPurchases() {
    auto pairs = analyticsService.getSharedPurchases();
    if (pairs.empty()) {
        cout << "Нет пар клиентов с общими покупками\n";
    } else {
        cout << "Клиенты, купившие одно и то же:\n";
        for (const auto& p : pairs) {
            cout << "- " << p.customer1 << " и " << p.customer2 << " → " << p.product << "\n";
        }
    }
}

// --- Analytics (новые) ---
void UserInterface::handleInactiveCustomers() {
    auto customers = analyticsService.getInactiveCustomers();
    if (customers.empty()) {
        cout << "Нет клиентов без заказов\n";
    } else {
        cout << "Клиенты без заказов:\n";
        for (const auto& c : customers) {
            cout << "- " << c.full_name << " (" << c.email << ")\n";
        }
    }
}

void UserInterface::handleAvgCheck() {
    auto checks = analyticsService.getAvgCheckPerCustomer();
    if (checks.empty()) {
        cout << "Нет данных\n";
    } else {
        cout << "Средний чек по клиентам:\n";
        cout << fixed << setprecision(2); // 2 знака после запятой
        for (const auto& a : checks) {
            cout << "- " << a.customer << ": " << a.avg_spent << " руб.\n";
        }
    }
}

void UserInterface::handlePopularProducts() {
    auto products = analyticsService.getPopularProducts();
    if (products.empty()) {
        cout << "Нет товаров, купленных более 1 раза\n";
    } else {
        cout << "Популярные товары (куплены >1 раза):\n";
        for (const auto& p : products) {
            cout << "- " << p.name << " (" << p.purchase_count << " раз(а))\n";
        }
    }
}

void UserInterface::handleRevenueByCategory() {
    auto categories = analyticsService.getRevenueByCategory();
    if (categories.empty()) {
        cout << "Нет данных\n";
    } else {
        cout << "Выручка по категориям:\n";
        for (const auto& c : categories) {
            cout << "- " << c.category << ": " << c.total_revenue << " руб.\n";
        }
    }
}

void UserInterface::handleSupplierSales() {
    auto suppliers = analyticsService.getSupplierSales();
    if (suppliers.empty()) {
        cout << "Нет данных\n";
    } else {
        cout << "Продажи по поставщикам:\n";
        for (const auto& s : suppliers) {
            cout << "- " << s.supplier << ": " << s.total_sales << " продаж\n";
        }
    }
}

// --- Usage ---
void UserInterface::printUsage() {
    cout << "Использование:\n";

    cout << "\nКатегории:\n";
    cout << "  add-category <название>\n";
    cout << "  delete-category <название>\n";
    cout << "  list-categories\n";

    cout << "\nПоставщики:\n";
    cout << "  add-supplier <название> <email>\n";
    cout << "  delete-supplier <email>\n";
    cout << "  list-suppliers\n";

    cout << "\nТовары:\n";
    cout << "  add-product --name <название> --price <цена> --category <категория> --supplier <email>\n";
    cout << "  list-products\n";

    cout << "\nКлиенты:\n";
    cout << "  add-customer <ФИО> <email>\n";
    cout << "  list-customers\n";

    cout << "\nЗаказы:\n";
    cout << "  create-order --customer <email> --product <название_товара>\n";
    cout << "  list-orders [--customer <email>]\n";

    cout << "\nАналитика:\n";
    cout << "  analytics top-products <N>\n";
    cout << "  analytics unused-suppliers\n";
    cout << "  analytics spending-report\n";
    cout << "  analytics expensive-products\n";
    cout << "  analytics shared-purchases\n";
    cout << "  analytics inactive-customers\n";
    cout << "  analytics avg-check\n";
    cout << "  analytics popular-products\n";
    cout << "  analytics revenue-by-category\n";
    cout << "  analytics supplier-sales\n";
}
