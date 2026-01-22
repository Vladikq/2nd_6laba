#include "AnalyticsService.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <string>

using namespace std;

AnalyticsService::AnalyticsService(DatabaseManager& database) : db(database) {}

vector<TopProduct> AnalyticsService::getTopProductsPerCategory(int limit) {
    string limitStr = to_string(limit);
    const char* params[1] = { limitStr.c_str() };
    PGresult* res = PQexecParams(db.getConnection(), R"(
        WITH ranked AS (
            SELECT
                c.name AS category,
                p.name AS product,
                p.price,
                ROW_NUMBER() OVER (PARTITION BY c.id ORDER BY p.price DESC) AS rn
            FROM products p
            JOIN categories c ON p.category_id = c.id
        )
        SELECT category, product, price
        FROM ranked
        WHERE rn <= $1
        ORDER BY category, price DESC;
    )", 1, nullptr, params, nullptr, nullptr, 0);

    vector<TopProduct> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            TopProduct tp;
            tp.category = string(PQgetvalue(res, i, 0));
            tp.product = string(PQgetvalue(res, i, 1));
            tp.price = stoi(string(PQgetvalue(res, i, 2)));
            result.push_back(tp);
        }
    }
    PQclear(res);
    return result;
}

vector<UnusedSupplier> AnalyticsService::getUnusedSuppliers() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT s.name, s.email
        FROM suppliers s
        WHERE NOT EXISTS (
            SELECT 1
            FROM products p
            JOIN orders o ON p.id = o.product_id
            WHERE p.supplier_id = s.id
        );
    )");

    vector<UnusedSupplier> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            UnusedSupplier us;
            us.name = string(PQgetvalue(res, i, 0));
            us.email = string(PQgetvalue(res, i, 1));
            result.push_back(us);
        }
    }
    PQclear(res);
    return result;
}

vector<CustomerSpending> AnalyticsService::getCustomerSpendingReport() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT
            cu.full_name,
            cu.email,
            COALESCE(SUM(p.price), 0)::INT
        FROM customers cu
        LEFT JOIN orders o ON cu.id = o.customer_id
        LEFT JOIN products p ON o.product_id = p.id
        GROUP BY cu.id, cu.full_name, cu.email
        ORDER BY SUM(p.price) DESC NULLS LAST;
    )");

    vector<CustomerSpending> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            CustomerSpending cs;
            cs.full_name = string(PQgetvalue(res, i, 0));
            cs.email = string(PQgetvalue(res, i, 1));
            cs.total_spent = stoi(string(PQgetvalue(res, i, 2)));
            result.push_back(cs);
        }
    }
    PQclear(res);
    return result;
}

vector<ExpensiveProduct> AnalyticsService::getProductsAboveCategoryAvg() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT p.name, p.price, c.name
        FROM products p
        JOIN categories c ON p.category_id = c.id
        WHERE p.price > (
            SELECT AVG(p2.price)
            FROM products p2
            WHERE p2.category_id = p.category_id
        )
        ORDER BY p.price DESC;
    )");

    vector<ExpensiveProduct> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            ExpensiveProduct ep;
            ep.product = string(PQgetvalue(res, i, 0));
            ep.price = stoi(string(PQgetvalue(res, i, 1)));
            ep.category = string(PQgetvalue(res, i, 2));
            result.push_back(ep);
        }
    }
    PQclear(res);
    return result;
}

vector<SharedPurchase> AnalyticsService::getSharedPurchases() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT DISTINCT
            c1.full_name,
            c2.full_name,
            p.name
        FROM orders o1
        JOIN orders o2 ON o1.product_id = o2.product_id AND o1.customer_id < o2.customer_id
        JOIN customers c1 ON o1.customer_id = c1.id
        JOIN customers c2 ON o2.customer_id = c2.id
        JOIN products p ON o1.product_id = p.id
        ORDER BY p.name, c1.full_name;
    )");

    vector<SharedPurchase> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            SharedPurchase sp;
            sp.customer1 = string(PQgetvalue(res, i, 0));
            sp.customer2 = string(PQgetvalue(res, i, 1));
            sp.product = string(PQgetvalue(res, i, 2));
            result.push_back(sp);
        }
    }
    PQclear(res);
    return result;
}

vector<InactiveCustomer> AnalyticsService::getInactiveCustomers() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT full_name, email
        FROM customers
        WHERE id NOT IN (SELECT DISTINCT customer_id FROM orders);
    )");

    vector<InactiveCustomer> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            InactiveCustomer c;
            c.full_name = string(PQgetvalue(res, i, 0));
            c.email = string(PQgetvalue(res, i, 1));
            result.push_back(c);
        }
    }
    PQclear(res);
    return result;
}

vector<AvgCheck> AnalyticsService::getAvgCheckPerCustomer() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT
            cu.full_name,
            ROUND(AVG(p.price), 2)
        FROM customers cu
        JOIN orders o ON cu.id = o.customer_id
        JOIN products p ON o.product_id = p.id
        GROUP BY cu.id, cu.full_name
        ORDER BY AVG(p.price) DESC;
    )");

    vector<AvgCheck> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            AvgCheck a;
            a.customer = string(PQgetvalue(res, i, 0));
            a.avg_spent = stod(string(PQgetvalue(res, i, 1)));
            result.push_back(a);
        }
    }
    PQclear(res);
    return result;
}

vector<PopularProduct> AnalyticsService::getPopularProducts() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT
            p.name,
            COUNT(o.id) AS cnt
        FROM products p
        JOIN orders o ON p.id = o.product_id
        GROUP BY p.id, p.name
        HAVING COUNT(o.id) > 1
        ORDER BY cnt DESC;
    )");

    vector<PopularProduct> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            PopularProduct p;
            p.name = string(PQgetvalue(res, i, 0));
            p.purchase_count = stoi(string(PQgetvalue(res, i, 1)));
            result.push_back(p);
        }
    }
    PQclear(res);
    return result;
}

vector<RevenueByCategory> AnalyticsService::getRevenueByCategory() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT
            c.name,
            SUM(p.price)
        FROM categories c
        JOIN products p ON c.id = p.category_id
        JOIN orders o ON p.id = o.product_id
        GROUP BY c.id, c.name
        ORDER BY SUM(p.price) DESC;
    )");

    vector<RevenueByCategory> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            RevenueByCategory r;
            r.category = string(PQgetvalue(res, i, 0));
            r.total_revenue = stoi(string(PQgetvalue(res, i, 1)));
            result.push_back(r);
        }
    }
    PQclear(res);
    return result;
}

vector<SupplierSales> AnalyticsService::getSupplierSales() {
    PGresult* res = PQexec(db.getConnection(), R"(
        SELECT
            s.name,
            COUNT(o.id)
        FROM suppliers s
        JOIN products p ON s.id = p.supplier_id
        JOIN orders o ON p.id = o.product_id
        GROUP BY s.id, s.name
        ORDER BY COUNT(o.id) DESC;
    )");

    vector<SupplierSales> result;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); ++i) {
            SupplierSales s;
            s.supplier = string(PQgetvalue(res, i, 0));
            s.total_sales = stoi(string(PQgetvalue(res, i, 1)));
            result.push_back(s);
        }
    }
    PQclear(res);
    return result;
}
