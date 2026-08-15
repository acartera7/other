
public class Product {
    public int Id { get; set; }
    public string Name { get; set; }
    public decimal Price { get; set; }
}

public interface IProductService {
    List<Product> GetAll();
    Product GetById(int id);
}

public class ProductService : IProductService {
    private readonly List<Product> _products = new() {
        new Product { Id = 1, Name = "Laptop", Price = 3500 },
        new Product { Id = 2, Name = "Mouse", Price = 120 },
        new Product { Id = 3, Name = "Keyboard", Price = 220 }
    };

    public List<Product> GetAll() => _products;

    public Product GetById(int id) =>
        _products.FirstOrDefault(p => p.Id == id)
        ?? throw new Exception("Product not found");
}
