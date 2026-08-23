using Microsoft.AspNetCore.Mvc;

public class BorrowRecord {
    public int Id { get; set; }
    public int BookId { get; set; }
    public string Borrower { get; set; }
    public DateTime BorrowedAt { get; set; }
    public DateTime? ReturnedAt { get; set; }
}

public class BorrowNotFoundException : Exception {
    public BorrowNotFoundException() { }
    public BorrowNotFoundException(string msg) : base(msg) { }
    public BorrowNotFoundException(string msg, Exception inner) : base(msg, inner) {}
}

public class BookNotAvailableException : Exception {
    public BookNotAvailableException() { }
    public BookNotAvailableException(string msg) : base(msg) { }
    public BookNotAvailableException(string msg, Exception inner) : base(msg, inner) {}
}

public class BorrowRequestDto {
    public int BookId { get; set; }
    public string Borrower { get; set; }
}

public class ReturnRequestDto {
    public int BorrowId { get; set; }
}

public interface IBorrowRepository {
    List<BorrowRecord> GetAll();
    BorrowRecord? GetById(int id);
    void Add(BorrowRecord record);
    bool Return(int id);
}

public class BorrowRepository : IBorrowRepository {
    private readonly LibraryDb _ctx;
    
    public BorrowRepository(LibraryDb ctx) {
        _ctx = ctx;
    }
    
    public List<BorrowRecord> GetAll() {
        return _ctx.BorrowRecords;
    }

    public BorrowRecord? GetById(int recordId) {
        return _ctx.BorrowRecords.FirstOrDefault(r => r.Id == recordId);
    }
    
    public void Add(BorrowRecord record) {
        _ctx.BorrowRecords.Add(record);
    }

    public bool Return(int id) {
        var record = GetById(id);
        if (record == null) return false;

        record.ReturnedAt = DateTime.Now;
        return true;
    }
}

public interface IBorrowService {
    List<BorrowRecord> GetAll();
    BorrowRecord Borrow(BorrowRequestDto dto);
    void Return(ReturnRequestDto dto);

}

public class BorrowService : IBorrowService {
    
    private readonly IBorrowRepository _repo;
    private readonly IBookService _bookService;
    
    public BorrowService(IBorrowRepository repo, IBookService bookService) {
        _repo = repo;
        _bookService = bookService;
    }
    
    public List<BorrowRecord> GetAll() => _repo.GetAll();
    
    public BorrowRecord Borrow(BorrowRequestDto dto) {
        // does the book exist?
        Book? book = _bookService.GetById(dto.BookId);
        if (book == null) 
            throw new BookNotFoundException($"Book {dto.BookId} not found");
        
        // check if we are trying to borrow and already borrowed book
        BorrowRecord? record = _repo.GetAll().FirstOrDefault(r => r.BookId == dto.BookId && r.ReturnedAt == null);
        if (record != null)
            throw new BookNotAvailableException($"Book {dto.BookId} is not available");
            
        //get next avaialbe Id
        IEnumerable<int> ids = _repo.GetAll().Select(r => r.Id);
        int nextId = Enumerable.Range(1, Int32.MaxValue).Except(ids).First();
        
        BorrowRecord newRecord = new BorrowRecord {
            Id = nextId, 
            BookId = book.Id, 
            Borrower = dto.Borrower, 
            BorrowedAt = DateTime.Now, 
            ReturnedAt = null
        };
        _repo.Add(newRecord);
        
        return newRecord;
    }
    
    public void Return(ReturnRequestDto dto) {
        BorrowRecord? record = _repo.GetById(dto.BorrowId);
        if (record == null)
            throw new BorrowNotFoundException($"Borrow {dto.BorrowId} not found");

        record.ReturnedAt = DateTime.Now;
    }
}

[ApiController]
[Route("api/[controller]")]
public class BorrowController : ControllerBase {
    private readonly IBorrowService _service;

    public BorrowController(IBorrowService service) {
        _service = service;
    }
    
    [HttpGet]
    public IActionResult GetAll() => Ok(_service.GetAll());

    [HttpPost]
    public IActionResult Borrow([FromBody] BorrowRequestDto dto) {
        var record = _service.Borrow(dto);
        return Ok(record);
    }

    [HttpPut("return")]
    public IActionResult Return([FromBody] ReturnRequestDto dto) {
        _service.Return(dto);
        return Ok();
    }
}