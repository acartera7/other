using Microsoft.AspNetCore.Mvc;

public class Book {
    public required int Id { get; set; } 
    public required string Title { get; set; } 
    public required string Author { get; set; } 
    public required int Year { get; set; } 
    public required string Genre { get; set; } 
}

public class BookNotFoundException : Exception {

    public BookNotFoundException() { }

    public BookNotFoundException(string msg) : base(msg) { }

    public BookNotFoundException(string msg, Exception inner) : base(msg, inner) {}
}

public interface IBookRepository {
    List<Book> GetAll();
    Book? GetById(int id);
    void Add(Book book);
    void Update(Book book);
    bool Delete(int id);
}

public class BookRepository : IBookRepository {
    private readonly LibraryDb _ctx;
    
    public BookRepository(LibraryDb ctx) {
        _ctx = ctx;
    }
    
    public List<Book> GetAll() {
        return _ctx.Books;
    }

    public Book? GetById(int id) {
        return _ctx.Books.FirstOrDefault(book => book.Id == id);
    }

    public void Add(Book book) {
        _ctx.Books.Add(book);
    }

    public void Update(Book book) {
        var index = _ctx.Books.FindIndex(b => b.Id == book.Id);
        if (index != -1) {
            _ctx.Books[index] = book;
        }
    }

    public bool Delete(int id) {
        Book? result = GetById(id);
        if (result != null) {
            return _ctx.Books.Remove(result);
            
        }
        return false;
    }
}

public interface IBookService {
    List<Book> GetAll();
    Book GetById(int id);
    List<Book> Search(string? title, string? author, string? genre);
    List<Book> GetSortedByYear(bool descending);
    void Add(Book book);
    void Update(Book book);
    void Delete(int id);
}

public class BookService : IBookService {
    
    private IBookRepository  _repo { get; set; }

    public BookService(IBookRepository  bookRepository) {
        _repo = bookRepository;
    }

    public List<Book> GetAll() => _repo.GetAll();
    
    public Book GetById(int id) {
        var book = _repo.GetById(id);
        if (book == null) {
            throw new BookNotFoundException($"Book {id} not found"); 
        }
        return book;
    }

    public List<Book> Search(string? title, string? author, string? genre) {
        return _repo.GetAll()
            .Where(b =>
                (string.IsNullOrEmpty(title) || b.Title.Contains(title, StringComparison.OrdinalIgnoreCase)) &&
                (string.IsNullOrEmpty(author) || b.Author.Contains(author, StringComparison.OrdinalIgnoreCase)) &&
                (string.IsNullOrEmpty(genre) || b.Genre.Contains(genre, StringComparison.OrdinalIgnoreCase))
            )
            .ToList();
    }
    
    public List<Book> GetSortedByYear(bool descending) {
        return descending
            ? _repo.GetAll().OrderByDescending(b => b.Year).ToList()
            : _repo.GetAll().OrderBy(b => b.Year).ToList();
    }

    public void Add(Book book) => _repo.Add(book);

    public void Update(Book book) {
        if (_repo.GetById(book.Id) == null) {
            throw new BookNotFoundException($"Book {book.Id} not found"); 
        }
        _repo.Update(book);
    }
    
    public void Delete(int id) {
        if (!_repo.Delete(id)) {
            throw new BookNotFoundException($"Book {id} not found");
        }
    }
}

[ApiController]
[Route("api/[controller]")]
public class BooksController : ControllerBase {
    private readonly IBookService _service;

    public BooksController(IBookService service) {
        _service = service;
    }

    [HttpGet()]
    public IActionResult GetAll() => Ok(_service.GetAll());

    [HttpGet("{id}")]
    public IActionResult GetById(int id) {
        try {
            return Ok(_service.GetById(id));
        }
        catch (BookNotFoundException ex) {
            return NotFound(ex.Message);
        }
    }

    [HttpGet("search")]
    public IActionResult Search(
        [FromQuery] string? title, 
        [FromQuery] string? author,
        [FromQuery] string? genre) => 
            Ok(_service.Search(title, author, genre));
    
    [HttpGet("sorted")]
    public IActionResult GetSortedByYear([FromQuery] bool descending) => 
        Ok(_service.GetSortedByYear(descending));

    [HttpPost]
    public IActionResult Add([FromBody] Book? book) {
        // Validate proper book
        if (book == null ||
            string.IsNullOrWhiteSpace(book.Title) ||
            string.IsNullOrWhiteSpace(book.Author)) {
            return BadRequest("Title and Author are required.");
        }
        _service.Add(book);
        return Ok(book);
    }
    
    [HttpPut("{id}")]
    public IActionResult Update(int id, [FromBody] Book? book) {
        // Validate proper book
        if (book == null || id != book.Id) {
            return BadRequest("Id in URL must match Id in body.");
        }

        try {
          _service.Update(book);
          return Ok(book);
        } catch (BookNotFoundException ex) {
            return NotFound(ex.Message);
        }
    }

    [HttpDelete("{id}")]
    public IActionResult Delete(int id) {
        try {
            _service.Delete(id);
            return Ok();
        }
        catch (BookNotFoundException ex) {
            return NotFound(ex.Message);
        }
    }
}