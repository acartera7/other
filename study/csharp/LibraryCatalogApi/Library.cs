using Microsoft.AspNetCore.Mvc;

public class Book {
    public int Id { get; set; }
    public string Title { get; set; }
    public string Author { get; set; }
    public int Year { get; set; }
    public string Genre { get; set; }
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
    private List<Book> _booklist { get; } = new();
    
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
        Book? result = _booklist.Find(b => b.Id == book.Id);
        if (result != null) {
            result = book;
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
    
    private BookRepository _repo { get; set; }

    public BookService(BookRepository bookRepository) {
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