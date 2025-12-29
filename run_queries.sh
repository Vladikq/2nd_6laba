#!/bin/bash

# Настройка подключения к PostgreSQL
export PGHOST=localhost
export PGPORT=5432
export PGDATABASE=shop_db
export PGUSER=postgres
export PGPASSWORD=password

echo "=== Инициализация базы данных ==="
psql -f init.sql
psql -f demo-data.sql

echo
echo "=== Тестирование приложения ==="

echo "1. Добавление категории:"
./shop_db add-category "Электроника"

echo
echo "2. Добавление поставщика:"
./shop_db add-supplier "TechCorp" "tech@corp.com"

echo
echo "3. Добавление товара:"
./shop_db add-product --name "Ноутбук" --price 50000 --category "Электроника" --supplier "tech@corp.com"

echo
echo "4. Добавление клиента:"
./shop_db add-customer "Иван Иванов" "ivan@mail.com"

echo
echo "5. Создание заказа:"
./shop_db create-order --customer "ivan@mail.com" --product "Ноутбук"

echo
echo "6. Список всех категорий:"
./shop_db list-categories

echo
echo "7. Список всех поставщиков:"
./shop_db list-suppliers

echo
echo "8. Список всех продуктов:"
./shop_db list-products

echo
echo "9. Список всех клиентов:"
./shop_db list-customers

echo
echo "10. Список всех заказов:"
./shop_db list-orders

echo
echo "=== Аналитические запросы ==="

echo "11. Топ-3 продукта по категориям:"
./shop_db analytics top-products 3

echo
echo "12. Неиспользуемые поставщики:"
./shop_db analytics unused-suppliers

echo
echo "13. Отчет по тратам клиентов:"
./shop_db analytics spending-report

echo
echo "14. Дорогие продукты (выше среднего в категории):"
./shop_db analytics expensive-products

echo
echo "15. Общие покупки клиентов:"
./shop_db analytics shared-purchases

echo
echo "16. Неактивные клиенты:"
./shop_db analytics inactive-customers

echo
echo "17. Средний чек по клиентам:"
./shop_db analytics avg-check

echo
echo "18. Популярные продукты:"
./shop_db analytics popular-products

echo
echo "19. Выручка по категориям:"
./shop_db analytics revenue-by-category

echo
echo "20. Продажи по поставщикам:"
./shop_db analytics supplier-sales
