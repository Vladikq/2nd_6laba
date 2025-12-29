-- === РОЛИ ЧЕМПИОНОВ (7 штук) ===
INSERT INTO categories (name) VALUES
  ('Assassin'),
  ('Fighter'),
  ('Mage'),
  ('Marksman'),
  ('Support'),
  ('Tank'),
  ('Specialist');

-- === РЕГИОНЫ РУНТЕРРЫ (8 штук) ===
INSERT INTO suppliers (name, email) VALUES
  ('Demacia', 'demacia@runeterra.com'),
  ('Noxus', 'noxus@runeterra.com'),
  ('Piltover', 'piltover@runeterra.com'),
  ('Zaun', 'zaun@runeterra.com'),
  ('Ionia', 'ionia@runeterra.com'),
  ('Freljord', 'freljord@runeterra.com'),
  ('Shurima', 'shurima@runeterra.com'),
  ('Void', 'void@runeterra.com');

-- === ЧЕМПИОНЫ (15 штук) ===
INSERT INTO products (name, price, category_id, supplier_id) VALUES
  -- Assassins
  ('Zed', 4800, (SELECT id FROM categories WHERE name = 'Assassin'), (SELECT id FROM suppliers WHERE email = 'ionia@runeterra.com')),
  ('Katarina', 3150, (SELECT id FROM categories WHERE name = 'Assassin'), (SELECT id FROM suppliers WHERE email = 'noxus@runeterra.com')),
  ('Akali', 6300, (SELECT id FROM categories WHERE name = 'Assassin'), (SELECT id FROM suppliers WHERE email = 'ionia@runeterra.com')),
  
  -- Fighters
  ('Garen', 450, (SELECT id FROM categories WHERE name = 'Fighter'), (SELECT id FROM suppliers WHERE email = 'demacia@runeterra.com')),
  ('Darius', 4800, (SELECT id FROM categories WHERE name = 'Fighter'), (SELECT id FROM suppliers WHERE email = 'noxus@runeterra.com')),
  ('Jax', 1350, (SELECT id FROM categories WHERE name = 'Fighter'), (SELECT id FROM suppliers WHERE email = 'ionia@runeterra.com')),
  
  -- Mages
  ('Lux', 3150, (SELECT id FROM categories WHERE name = 'Mage'), (SELECT id FROM suppliers WHERE email = 'demacia@runeterra.com')),
  ('Jinx', 6300, (SELECT id FROM categories WHERE name = 'Marksman'), (SELECT id FROM suppliers WHERE email = 'zaun@runeterra.com')),
  
  -- Tanks
  ('Braum', 6300, (SELECT id FROM categories WHERE name = 'Tank'), (SELECT id FROM suppliers WHERE email = 'freljord@runeterra.com')),
  ('Leona', 4800, (SELECT id FROM categories WHERE name = 'Tank'), (SELECT id FROM suppliers WHERE email = 'demacia@runeterra.com')),
  
  -- Marksmen
  ('Ashe', 450, (SELECT id FROM categories WHERE name = 'Marksman'), (SELECT id FROM suppliers WHERE email = 'freljord@runeterra.com')),
  ('Caitlyn', 4800, (SELECT id FROM categories WHERE name = 'Marksman'), (SELECT id FROM suppliers WHERE email = 'piltover@runeterra.com')),
  
  -- Support
  ('Soraka', 450, (SELECT id FROM categories WHERE name = 'Support'), (SELECT id FROM suppliers WHERE email = 'ionia@runeterra.com')),
  
  -- Mage
  ('Azir', 6300, (SELECT id FROM categories WHERE name = 'Mage'), (SELECT id FROM suppliers WHERE email = 'shurima@runeterra.com')),
  
  -- Specialist (Void)
  ('Kassadin', 3150, (SELECT id FROM categories WHERE name = 'Specialist'), (SELECT id FROM suppliers WHERE email = 'void@runeterra.com'));

-- === ПРИЗЫВАТЕЛИ (7 штук) ===
INSERT INTO customers (full_name, email) VALUES
  ('Анна Иванова', 'anna@summoner.com'),
  ('Николай Петров', 'nikolai@summoner.com'),
  ('Мария Сидорова', 'maria@summoner.com'),
  ('Сергей Козлов', 'sergey@summoner.com'),
  ('Елена Волкова', 'elena@summoner.com'),
  ('Дмитрий Орлов', 'dmitry@summoner.com'),
  ('Ольга Морозова', 'olga@summoner.com'),
  ('Призыватель Без Чемпионов', 'noorders@summoner.com');

-- === ПОКУПКИ ЧЕМПИОНОВ (12 штук) ===
INSERT INTO orders (customer_id, product_id, order_date, status) VALUES
  -- Анна
  ((SELECT id FROM customers WHERE email = 'anna@summoner.com'), (SELECT id FROM products WHERE name = 'Jinx'), '2025-12-03', 'delivered'),
  ((SELECT id FROM customers WHERE email = 'anna@summoner.com'), (SELECT id FROM products WHERE name = 'Katarina'), '2025-12-04', 'delivered'),
  ((SELECT id FROM customers WHERE email = 'anna@summoner.com'), (SELECT id FROM products WHERE name = 'Ashe'), '2025-12-08', 'pending'),
  
  -- Николай
  ((SELECT id FROM customers WHERE email = 'nikolai@summoner.com'), (SELECT id FROM products WHERE name = 'Darius'), '2025-12-05', 'delivered'),
  ((SELECT id FROM customers WHERE email = 'nikolai@summoner.com'), (SELECT id FROM products WHERE name = 'Braum'), '2025-12-09', 'delivered'),
  
  -- Мария
  ((SELECT id FROM customers WHERE email = 'maria@summoner.com'), (SELECT id FROM products WHERE name = 'Soraka'), '2025-12-06', 'delivered'),
  ((SELECT id FROM customers WHERE email = 'maria@summoner.com'), (SELECT id FROM products WHERE name = 'Caitlyn'), '2025-12-07', 'pending'),
  
  -- Сергей
  ((SELECT id FROM customers WHERE email = 'sergey@summoner.com'), (SELECT id FROM products WHERE name = 'Azir'), '2025-12-11', 'pending'),
  
  -- Елена
  ((SELECT id FROM customers WHERE email = 'elena@summoner.com'), (SELECT id FROM products WHERE name = 'Jax'), '2025-12-12', 'delivered'),
  ((SELECT id FROM customers WHERE email = 'elena@summoner.com'), (SELECT id FROM products WHERE name = 'Leona'), '2025-12-13', 'pending'),
  
  -- Дмитрий
  ((SELECT id FROM customers WHERE email = 'dmitry@summoner.com'), (SELECT id FROM products WHERE name = 'Akali'), '2025-12-14', 'delivered'),
  
  -- Ольга
  ((SELECT id FROM customers WHERE email = 'olga@summoner.com'), (SELECT id FROM products WHERE name = 'Kassadin'), '2025-12-15', 'pending');
