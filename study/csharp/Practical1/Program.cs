using System;
using System.IO;
using System.Text.Json;
using System.Collections.Generic;

      
namespace Practical1;

class Program {
   public static void Main(string[] args) {
      Library lib = new Library();
      
      //lib.AddBook(new Book("1984", "George Orwell", 1949));
      //lib.AddBook(new Book("To Kill a Mockingbird", "Harper Lee", 1960));
      //lib.AddBook(new Book("The Da Vinci Code", "Dan Brown", 2003));
      //lib.AddBook(new Book("The Great Gatsby", "F. Scott Fitzgerald", 1925));
      //lib.AddBook(new Book("Lord of the Flies", "William Golding", 1954));

      //var options = new JsonSerializerOptions { WriteIndented = true};
      //string jsonString = JsonSerializer.Serialize(lib.List(), options);
      //File.WriteAllText(filePath, jsonString);

      string filePath = "../../../library.json";
      string json = File.ReadAllText(filePath);
      List<Book>? books = JsonSerializer.Deserialize<List<Book>>(json);
      
      if (books != null)
      {
         foreach (Book book in books)
         {
            lib.AddBook(book);
         }
      }

      lib.BorrowBook("Inferno");
      lib.BorrowBook("The War of the Worlds");
      lib.BorrowBook("The Call of Cthulhu");
      lib.BorrowBook("Frankenstein");
      
      Console.WriteLine("===== GetBooksByAuthor (Dan Brown) =====");
      var query = lib.GetBooksByAuthor("Dan Brown");
      foreach (var b in query) {
         Console.WriteLine(b.ToString());
      }
      Console.WriteLine("");
      
      query = lib.GetBorrowedBooks();
      Console.WriteLine("===== GetBorrowedBooks =====");
      foreach (var b in query) {
         Console.WriteLine(b.ToString());
      }
      Console.WriteLine("");

      query = lib.GetAvailableBooks();
      Console.WriteLine("===== GetAvailableBooks =====");
      foreach (var b in query) {
         Console.WriteLine(b.ToString());
      }
      Console.WriteLine("");

      Console.WriteLine("===== SearchByTitle (The ) =====");
      query = lib.SearchByTitle("The ");
      foreach (var b in query) {
         Console.WriteLine(b.ToString());
      }
      Console.WriteLine("");
      
      Console.WriteLine("===== GetBooksSortedByYear (descending) =====");
      query = lib.GetBooksSortedByYear(true);
      foreach (var b in query) {
         Console.WriteLine(b.ToString());
      }
      Console.WriteLine("");
      
      Console.WriteLine("===== GetBooksGroupedByAuthor =====");
      var result = lib.GetBooksGroupedByAuthor();
      foreach (var group in result) {
         Console.WriteLine($"{group.Key}: ");
         
         foreach (var b in group) {
         
            Console.WriteLine($"{b.Title}, {b.Year}");
         }
         Console.WriteLine("");
         
      }
   }
}