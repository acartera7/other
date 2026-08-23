using Microsoft.AspNetCore.Mvc;

public class BorrowRecord {
    public int Id { get; set; }
    public int BookId { get; set; }
    public string Borrower { get; set; }
    public DateTime BorrowedAt { get; set; }
    public DateTime? ReturnedAt { get; set; }
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

    public BorrowRecord? GetById(int id) {
        return _ctx.BorrowRecords.FirstOrDefault(r => r.Id == id);
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
    Task<List<BorrowRecord>> GetAll() {
        
    }

    Task<BorrowRecord> Borrow(BorrowRequestDto dto) {
        
    }

    Task Return(ReturnRequestDto dto) {
        
    }
}