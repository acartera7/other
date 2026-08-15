using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Dynamic;
using System.Linq;

namespace Project2;

class Product {
    public int ID { get; }
    public string Name { get; } 
    public decimal Price { get; }

    public Product(int id, string name, decimal price) {
        ID = id;
        Name = name;
        Price = price;
    }
}

class OrderItem {
    public Product Product { get; }
    public int Quantity { get; }

    public OrderItem(Product product, int quantity) {
        Product = product;
        Quantity = quantity;
    }
}

class Order {
    public int ID { get; }
    private List<OrderItem> _items { get; } = new();

    public IReadOnlyList<OrderItem> Items() => _items;
        
    public Order(int id, List<OrderItem> items) {
        ID = id;
        _items = items;
    }

    public decimal GetTotal() {
        if (_items.Count != 0) {
            return _items.Sum(item => item.Quantity * item.Product.Price);
        }

        return 0.0m;
    }
}

class OrderNotFoundException : Exception {
    public OrderNotFoundException() { }
    public OrderNotFoundException(string message) : base(message) { }
    public OrderNotFoundException(string message, Exception inner) : base(message, inner) { }
}

class OrderService {
    public List<Order> OrdersList { get; private set; } = new();
    
    public void AddOrder(Order order) {
        OrdersList.Add(order);
    }
    
    public bool RemoveOrder(Order order) {
        return OrdersList.Remove(order);
    }

    public Order? GetOrderById(int id) {
        return OrdersList.Find(order => order.ID == id) ?? throw new OrderNotFoundException("Order Id doesn't exist");
    }

    public List<Order> GetOrdersAboveAmount(decimal amount) {
        return OrdersList.Where(order => order.GetTotal() > amount).ToList();
    }
}

class Program {
    public static void Main(string[] args) {

        var ff = new Product(1000, "French Fries", 3.99m);
        var fd = new Product(1001, "Fountain Drink", 2.99m);
        var cb = new Product(1002, "Cheeseburger", 6.95m);
        var cs = new Product(1007, "Chocolate Shake", 4.95m);
        var ss = new Product(1008, "Strawberry Shake", 4.95m);
        var bb = new Product(1005, "Bacon Burger", 7.95m);
        
        Order o1 = new Order(1, new List<OrderItem>{
            new OrderItem( ff, 2),
            new OrderItem( cb,1),
            new OrderItem( cs,1)
        });

        Order o2 = new Order(2, new List<OrderItem> {
           new OrderItem(fd ,1), 
           new OrderItem(bb ,1), 
        });

        OrderService orderService = new OrderService();
        
        orderService.AddOrder(o1);
        orderService.AddOrder(o2);

        foreach (var order in orderService.OrdersList) {
            Console.WriteLine($"Order {order.ID} Total: {order.GetTotal()}");
        }
        Console.WriteLine("\n");
        
        Console.WriteLine($"Orders above 15: ");
        foreach (var order in orderService.GetOrdersAboveAmount(15)) {
            Console.WriteLine($"Order {order.ID} Total: {order.GetTotal()}");
        }
    }

}