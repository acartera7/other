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