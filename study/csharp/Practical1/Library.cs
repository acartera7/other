using System.Collections.Generic;

namespace Practical1;

public class Book {
   public string Title { get; }
   public string Author { get; }
   public int Year { get; }
   public bool IsBorrowed { get; set; }

   public Book(string title, string author, int year) {
      Title = title;
      Author = author;
      Year = year;
      IsBorrowed = false;
   }
   
   public override string ToString() {
      return $"{Title}, {Author}, {Year}, Borrowed: {IsBorrowed}";
   }
   
}

public class Library {
   private List<Book> _booklist = new();

   public void AddBook(Book book) {
      _booklist.Add(book);
   }

   public bool BorrowBook(string title) {
      Book? book = _booklist.Find(book => book.Title == title);
      
      if (book != null && !book.IsBorrowed) {
         book.IsBorrowed = true;
         return true;
      }
      
      return false;
   }

   public bool ReturnBook(string title) {
      Book? book = _booklist.Find(book => book.Title == title);

      if (book != null && book.IsBorrowed) {
         book.IsBorrowed = false;
         return true;
      }

      return false;
   }

   public IReadOnlyList<Book> List() {
      return _booklist;
   }
   
   // === LINQ ===
   public IEnumerable<Book> GetBooksByAuthor(string author) {
      return _booklist.FindAll(b => b.Author == author);
   }
}

