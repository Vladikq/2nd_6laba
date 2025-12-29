CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -I/usr/include/postgresql
LDFLAGS = -lpq

SOURCES = main.cpp \
          DatabaseManager.cpp \
          repositories/CategoryService.cpp \
          repositories/SupplierService.cpp \
          repositories/ProductService.cpp \
          repositories/CustomerService.cpp \
          repositories/OrderService.cpp \
          repositories/AnalyticsService.cpp \
          ui/UserInterface.cpp

EXECUTABLE = shop_db

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)

.PHONY: clean
