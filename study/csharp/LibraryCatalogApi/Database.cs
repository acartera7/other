    
public class LibraryDb {
    
    public List<Book> Books { get; } = new()
    {
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

    public List<BorrowRecord> BorrowRecords { get; } = new()
    {
        new BorrowRecord { Id = 1, BookId = 1, Borrower = "Alice Johnson", BorrowedAt = new DateTime(2026, 7, 10), ReturnedAt = new DateTime(2026, 7, 24) },
        new BorrowRecord { Id = 2, BookId = 3, Borrower = "Bob Smith", BorrowedAt = new DateTime(2026, 8, 1), ReturnedAt = null },
        new BorrowRecord { Id = 3, BookId = 5, Borrower = "Charlie Brown", BorrowedAt = new DateTime(2026, 6, 15), ReturnedAt = new DateTime(2026, 6, 29) },
        new BorrowRecord { Id = 4, BookId = 7, Borrower = "Diana Miller", BorrowedAt = new DateTime(2026, 8, 5), ReturnedAt = null },
        new BorrowRecord { Id = 5, BookId = 10, Borrower = "Ethan Wilson", BorrowedAt = new DateTime(2026, 7, 20), ReturnedAt = new DateTime(2026, 8, 3) },
        new BorrowRecord { Id = 6, BookId = 17, Borrower = "Fiona Davis", BorrowedAt = new DateTime(2026, 8, 10), ReturnedAt = null },
        new BorrowRecord { Id = 7, BookId = 18, Borrower = "George Taylor", BorrowedAt = new DateTime(2026, 5, 12), ReturnedAt = new DateTime(2026, 5, 26) },
        new BorrowRecord { Id = 8, BookId = 22, Borrower = "Hannah Moore", BorrowedAt = new DateTime(2026, 8, 8), ReturnedAt = null },
        new BorrowRecord { Id = 9, BookId = 27, Borrower = "Ian Anderson", BorrowedAt = new DateTime(2026, 7, 1), ReturnedAt = new DateTime(2026, 7, 15) },
        new BorrowRecord { Id = 10, BookId = 30, Borrower = "Julia Thomas", BorrowedAt = new DateTime(2026, 8, 12), ReturnedAt = null },
        new BorrowRecord { Id = 11, BookId = 33, Borrower = "Kevin Jackson", BorrowedAt = new DateTime(2026, 6, 20), ReturnedAt = new DateTime(2026, 7, 4) },
        new BorrowRecord { Id = 12, BookId = 35, Borrower = "Laura White", BorrowedAt = new DateTime(2026, 8, 15), ReturnedAt = null }
    };
}
