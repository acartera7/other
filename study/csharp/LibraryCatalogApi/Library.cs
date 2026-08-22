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
    private List<Book> _booklist { get; } = new() {
        new Book { Id = 1, Title = "1984", Author = "George Orwell", Year = 1949, Genre = "Dystopian" },
        new Book { Id = 2, Title = "To Kill a Mockingbird", Author = "Harper Lee", Year = 1960, Genre = "Classic Fiction" },
        new Book { Id = 3, Title = "The Da Vinci Code", Author = "Dan Brown", Year = 2003, Genre = "Mystery Thriller" },
        new Book { Id = 4, Title = "The Great Gatsby", Author = "F. Scott Fitzgerald", Year = 1925, Genre = "Classic Fiction" },
        new Book { Id = 5, Title = "Lord of the Flies", Author = "William Golding", Year = 1954, Genre = "Allegorical Fiction" },
        new Book { Id = 6, Title = "Animal Farm", Author = "George Orwell", Year = 1945, Genre = "Political Satire" },
        new Book { Id = 7, Title = "Homage to Catalonia", Author = "George Orwell", Year = 1938, Genre = "Memoir / History" },
        new Book { Id = 8, Title = "Go Set a Watchman", Author = "Harper Lee", Year = 2015, Genre = "Classic Fiction" },
        new Book { Id = 9, Title = "Angels & Demons", Author = "Dan Brown", Year = 2000, Genre = "Mystery Thriller" },
        new Book { Id = 10, Title = "Inferno", Author = "Dan Brown", Year = 2013, Genre = "Mystery Thriller" },
        new Book { Id = 11, Title = "The Lost Symbol", Author = "Dan Brown", Year = 2009, Genre = "Mystery Thriller" },
        new Book { Id = 12, Title = "Digital Fortress", Author = "Dan Brown", Year = 1998, Genre = "Techno-thriller" },
        new Book { Id = 13, Title = "This Side of Paradise", Author = "F. Scott Fitzgerald", Year = 1920, Genre = "Classic Fiction" },
        new Book { Id = 14, Title = "Tender Is the Night", Author = "F. Scott Fitzgerald", Year = 1934, Genre = "Classic Fiction" },
        new Book { Id = 15, Title = "The Inheritors", Author = "William Golding", Year = 1955, Genre = "Historical Fiction" },
        new Book { Id = 16, Title = "The War of the Worlds", Author = "H. G. Wells", Year = 1898, Genre = "Science Fiction" },
        new Book { Id = 17, Title = "The Hobbit", Author = "J. R. R. Tolkien", Year = 1937, Genre = "Fantasy" },
        new Book { Id = 18, Title = "The Fellowship of the Ring", Author = "J. R. R. Tolkien", Year = 1954, Genre = "Fantasy" },
        new Book { Id = 19, Title = "The Two Towers", Author = "J. R. R. Tolkien", Year = 1954, Genre = "Fantasy" },
        new Book { Id = 20, Title = "The Return of the King", Author = "J. R. R. Tolkien", Year = 1955, Genre = "Fantasy" },
        new Book { Id = 21, Title = "The Silmarillion", Author = "J. R. R. Tolkien", Year = 1977, Genre = "Fantasy" },
        new Book { Id = 22, Title = "The Call of Cthulhu", Author = "H. P. Lovecraft", Year = 1928, Genre = "Horror" },
        new Book { Id = 23, Title = "At the Mountains of Madness", Author = "H. P. Lovecraft", Year = 1936, Genre = "Horror" },
        new Book { Id = 24, Title = "The Shadow over Innsmouth", Author = "H. P. Lovecraft", Year = 1936, Genre = "Horror" },
        new Book { Id = 25, Title = "The Dunwich Horror", Author = "H. P. Lovecraft", Year = 1929, Genre = "Horror" },
        new Book { Id = 26, Title = "The Colour Out of Space", Author = "H. P. Lovecraft", Year = 1927, Genre = "Horror" },
        new Book { Id = 27, Title = "Dracula", Author = "Bram Stoker", Year = 1897, Genre = "Gothic Horror" },
        new Book { Id = 28, Title = "Frankenstein", Author = "Mary Shelley", Year = 1818, Genre = "Gothic Horror / Sci-Fi" },
        new Book { Id = 29, Title = "The Picture of Dorian Gray", Author = "Oscar Wilde", Year = 1890, Genre = "Philosophical Fiction" },
        new Book { Id = 30, Title = "The Count of Monte Cristo", Author = "Alexandre Dumas", Year = 1844, Genre = "Adventure / Historical" },
        new Book { Id = 31, Title = "Treasure Island", Author = "Robert Louis Stevenson", Year = 1883, Genre = "Adventure" },
        new Book { Id = 32, Title = "Moby-Dick", Author = "Herman Melville", Year = 1851, Genre = "Classic Fiction / Adventure" },
        new Book { Id = 33, Title = "Pride and Prejudice", Author = "Jane Austen", Year = 1813, Genre = "Classic Romance" },
        new Book { Id = 34, Title = "Jane Eyre", Author = "Charlotte Brontë", Year = 1847, Genre = "Gothic Romance" },
        new Book { Id = 35, Title = "The Time Machine", Author = "H. G. Wells", Year = 1895, Genre = "Science Fiction" },
        new Book { Id = 36, Title = "The Invisible Man", Author = "H. G. Wells", Year = 1897, Genre = "Science Fiction" }
    };
    
    public List<Book> GetAll() {
        return _booklist;
    }

    public Book? GetById(int id) {
        return _booklist.FirstOrDefault(book => book.Id == id);
    }

    public void Add(Book book) {
        _booklist.Add(book);
    }

    public void Update(Book book) {
        var index = _booklist.FindIndex(b => b.Id == book.Id);
        if (index != -1) {
            _booklist[index] = book;
        }
    }

    public bool Delete(int id) {
        Book? result = GetById(id);
        if (result != null) {
            return _booklist.Remove(result);
            
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