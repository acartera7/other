
using Microsoft.AspNetCore.Mvc;

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
        new Product { Id = 3, Name = "Keyboard", Price = 220 },
        new Product { Id = 4, Name = "Monitor", Price = 1200 },
        new Product { Id = 5, Name = "Headphones", Price = 350 },
        new Product { Id = 6, Name = "External Hard Drive", Price = 450 },
        new Product { Id = 7, Name = "Webcam", Price = 280 },
        new Product { Id = 8, Name = "Smartphone", Price = 4200 },
        new Product { Id = 9, Name = "Graphic Tablet", Price = 850 },
        new Product { Id = 10, Name = "Microphone", Price = 400 },
        new Product { Id = 11, Name = "Desk Mat", Price = 75 },
        new Product { Id = 12, Name = "USB-C Hub", Price = 190 },
        new Product { Id = 13, Name = "Smart Watch", Price = 1500 },
        new Product { Id = 14, Name = "Router", Price = 600 },
        new Product { Id = 15, Name = "Gaming Chair", Price = 1800 }
    };

    public List<Product> GetAll() => _products;

    public Product GetById(int id) =>
        _products.FirstOrDefault(p => p.Id == id)
        ?? throw new Exception("Product not found");
}


[ApiController]
[Route("api/[controller]")]
public class ProductsController : ControllerBase {
    private readonly IProductService _service;

    public ProductsController(IProductService service) {
        _service = service;
    }

    [HttpGet]
    public IActionResult GetAll() => Ok(_service.GetAll());

    [HttpGet("{id}")]
    public IActionResult GetById(int id) => Ok(_service.GetById(id));
}