namespace LibraryCatalogApi;

public class Book {
    public int Id { get; set; }
    public string Title { get; set; }
    public string Author { get; set; }
    public int Year { get; set; }
    public string Genre { get; set; }
}

public interface IBookRepository {
    List<Book> GetAll();
    Book? GetById(int id);
    void Add(Book book);
    void Update(Book book);
    bool Delete(int id);
}

public class BookRepository : IBookRepository {
    private List<Book> _booklist { get; set; } = new();
    
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
        Book? result = _booklist.FirstOrDefault(b => b.Id == id);
        if (result != null) {
            return _booklist.Remove(result);
            
        }

        return false;
    }
}