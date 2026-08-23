const borrowedBooks = {};

async function loadBooks() {
    const response = await fetch("http://localhost:5124/api/books");
    const books = await response.json();

    const container = document.getElementById("books");
    container.innerHTML = "";
    
    books.forEach(b => {
        const row = document.createElement("tr");

        row.innerHTML = `
            <td>${b.id}</td>
            <td>${b.title}</td>
            <td>${b.author}</td>
            <td>${b.year}</td>
            <td>${b.genre}</td>
        `;

        container.appendChild(row);
    });
}

async function searchBooks(query) {
    const response = await fetch(`http://localhost:5124/api/books/search?${query}`)
    const books = await response.json();

    const container = document.getElementById("searches");
    container.innerHTML = "";

    books.forEach(b => {
        const row = document.createElement("tr");

        row.innerHTML = `
            <td>${b.id}</td>
            <td>${b.title}</td>
            <td>${b.author}</td>
            <td>${b.year}</td>
            <td>${b.genre}</td>
        `;

        container.appendChild(row);
    });
}

async function sortBooks(descending) {
    const response = await fetch(`http://localhost:5124/api/books/sorted?descending=${descending}`)
    const books = await response.json();

    const container = document.getElementById("sorted");
    container.innerHTML = "";

    books.forEach(b => {
        const row = document.createElement("tr");

        row.innerHTML = `
            <td>${b.id}</td>
            <td>${b.title}</td>
            <td>${b.author}</td>
            <td>${b.year}</td>
            <td>${b.genre}</td>
        `;

        container.appendChild(row);
    });
}



async function borrowBook(bookId, borrower){
    const response = await fetch("http://localhost:5124/api/borrow",{
        method: "POST",
        headers: { "Content-Type": "application/json"},
        body: JSON.stringify({ bookId, borrower})
    });
    
    if (!response.ok) {
        console.error("Borrow failed:", response.status, response.statusText);
        return;
    }
    
    const result = await response.json();
    // Save the API-generated borrowId
    borrowedBooks[bookId] = result.id;

    console.log("Borrowed:", result);
    console.log("Saved borrowId:", borrowedBooks[bookId]);
    await loadBorrowed();
}

async function returnBook(bookId) {

    //Find the borrowId associated with this book
    const borrowId = borrowedBooks[bookId];

    if (borrowId === undefined) {
        console.error(`Book ${bookId} is not currently borrowed.`);
        return;
    }
    
    const response = await fetch("http://localhost:5124/api/borrow/return", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ borrowId })
    });

    if (!response.ok) {
        console.error("Return failed:", response.status, response.statusText);
        return;
    }
    
    delete borrowedBooks[bookId];

    console.log("Returned book #: ", bookId);
    console.log("Borrow Id: ", borrowId);
    await loadBorrowed();
}

async function loadBorrowed() {
    const response = await fetch("http://localhost:5124/api/borrow")
    const records = await response.json();
    
    const container = document.getElementById("borrowed");
    container.innerHTML = "";

    records.forEach(r => {
        const div = document.createElement("div");
        div.textContent = `${r.borrower} borrowed book ${r.bookId} on ${r.borrowedAt}`;
        container.appendChild(div);
    });
}