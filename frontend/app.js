let isLoggedIn = false;
let username = "";
let cash = 10000;
let portfolio = {};

function updateUI() {
  document.getElementById("auth-section").style.display = isLoggedIn ? "none" : "block";
  document.getElementById("app-section").style.display = isLoggedIn ? "block" : "none";
  document.getElementById("user-display").innerText = username;
  document.getElementById("cash").innerText = cash.toFixed(2);
  const tbody = document.querySelector("#portfolio-table tbody");
  tbody.innerHTML = "";

  for (const sym in portfolio) {
    const row = document.createElement("tr");
    row.innerHTML = `<td>${sym}</td><td>${portfolio[sym].shares}</td><td>${portfolio[sym].avgPrice.toFixed(2)}</td>`;
    tbody.appendChild(row);
  }
}

async function signUp() {
    const res = await fetch("/signup", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        username: document.getElementById("username").value,
        password: document.getElementById("password").value,
      })
    });
    const result = await res.json();
    console.log("✅ SignUp Response:", result);
    document.getElementById("auth-message").innerText = result.output;
  }
  

  async function logIn() {
    const res = await fetch("/login", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        username: document.getElementById("username").value,
        password: document.getElementById("password").value,
      })
    });
  
    const result = await res.json();
    if (result.output.includes("success")) {
      isLoggedIn = true;
      username = document.getElementById("username").value;
      await fetchPortfolio();
      updateUI();
    } else {
      alert("❌ Login failed: " + result.output);
    }
  }
  

async function buyStock() {
  const sym = document.getElementById("buy-symbol").value.toUpperCase();
  const shares = parseInt(document.getElementById("buy-shares").value);
  const date = prompt("Enter date (YYYY-MM-DD):");

  const res = await fetch("/buy", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ username, symbol: sym, shares, date })
  });

  const result = await res.json();
  alert(result.output);
  await fetchPortfolio();
  updateUI();
}


async function sellStock() {
    const sym = document.getElementById("sell-symbol").value.toUpperCase();
    const shares = parseInt(document.getElementById("sell-shares").value);
    const date = prompt("Enter date (YYYY-MM-DD):");
  
    const res = await fetch("/sell", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ username, symbol: sym, shares, date })
    });
  
    const result = await res.json();
    alert(result.output);
    await fetchPortfolio();
    updateUI();
}

async function fetchPortfolio() {
  const res = await fetch(`/portfolio?username=${username}`);
  const result = await res.json();

  portfolio = {};
  // cash = 10000; ❌ REMOVE THIS LINE

  const lines = result.output.split("\n");
  for (const line of lines) {
    console.log("📦 Line:", line); // Debug
    const [sym, shares, avg] = line.trim().split(",");
    if (sym === "CASH") {
      cash = parseFloat(shares); // ✅ This updates cash
      console.log("💵 Parsed cash:", cash);
    } else if (sym && shares && avg) {
      portfolio[sym] = {
        shares: parseInt(shares),
        avgPrice: parseFloat(avg)
      };
    }
  }
}

  
  

