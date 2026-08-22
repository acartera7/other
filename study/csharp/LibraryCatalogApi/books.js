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