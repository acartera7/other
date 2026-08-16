async function loadProducts() {
    const response = await fetch("http://localhost:5070/api/products");
    const products = await response.json();
    
    const container = document.getElementById("products");
    container.innerHTML = "";
    
    products.forEach(p => {
        const div = document.createElement("div");
        div.className = "product";
        div.textContent = `${p.id}: ${p.name} - ${p.price} RON`;
        container.appendChild(div); 
    });
}

async function loadBooks() {
    const response = await fetch("https://dummyjson.com/c/6e65-fede-4e2a-b8dc");
    const books = await response.json();

    const container = document.getElementById("products");
    container.innerHTML = "";

    books.forEach(b => {
        const div = document.createElement("div");
        div.className = "books";
        div.textContent = `${b.Title} - ${b.Author}, ${b.Year}, ${b.Genre}`;
        container.appendChild(div);
    });
}