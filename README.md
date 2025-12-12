# Inventory Management and Shopping Cart System

## 📋 Description

A C-based inventory management system for retail branches with integrated shopping cart functionality. The program manages product catalogs, multi-branch inventory tracking, shopping cart operations, and purchase finalization with automatic stock availability verification.

**Developed by:**
-> Luiz Henrique Leite


---

## ✨ Features

- ✅ Product catalog with search by code or description
- ✅ Multiple branch management with independent inventories
- ✅ Shopping cart with add/remove item functionality
- ✅ Automatic availability verification across branches
- ✅ Smart cart adjustment for partial stock availability
- ✅ Purchase finalization with automatic stock deduction
- ✅ Transaction history and receipt generation
- ✅ Batch command processing from file

---

## 🛠️ Technologies Used

- **Language:** C
- **Data Structures:** Linked Lists
- **File I/O:** Text file processing
- **Memory Management:** Dynamic allocation with malloc/free

---

## 📦 Project Structure

```
.
├── main.c                          # Main source code
├── produtos_v2.txt                 # Product catalog
├── filiais_v2.txt                  # Branch information
├── estoques_v2.txt                 # Inventory data
├── carrinhos_clientes_v2.txt       # Batch commands (optional)
├── ultima_compra.txt               # Last purchase receipt (generated)
└── relatorio_geral.txt             # Transaction history (generated)
```

---

## 📄 Input File Formats

### produtos_v2.txt
```
<code> <description> <price>
```
Example:
```
101 Aspirin 12.50
102 Bandage 8.30
103 Vitamins 25.00
```

### filiais_v2.txt
```
<branch_id> <name> <city>
```
Example:
```
1 MainBranch NewYork
2 WestBranch LosAngeles
3 EastBranch Boston
```

### estoques_v2.txt
```
<branch_id> <product_code> <quantity>
```
Example:
```
1 101 50
1 102 100
2 101 30
```

### carrinhos_clientes_v2.txt (Optional - for batch processing)
```
NOVO_CARRINHO <cart_id>
ADD <product_code> <quantity>
REMOVE <product_code> <quantity>
VERIFICAR_ATENDIMENTO
FINALIZAR <branch_id>
FIM
```

**Command Reference:**
- `NOVO_CARRINHO <id>` - Creates a new shopping cart with specified ID
- `ADD <code> <qty>` - Adds product to cart
- `REMOVE <code> <qty>` - Removes specified quantity from cart
- `VERIFICAR_ATENDIMENTO` - Checks which branches can fulfill the order
- `FINALIZAR <branch_id>` - Finalizes purchase at specified branch
- `FIM` - End of commands

**Real Example:**
```
NOVO_CARRINHO 1
ADD 2001 2
VERIFICAR_ATENDIMENTO
FINALIZAR 101
FIM
```
This creates cart #1, adds 2 units of product 2001, checks availability, and completes purchase at branch 101.

---

## 🚀 How to Compile and Run

### Compilation

```bash
gcc main.c -o inventory_system
```

Or with warnings enabled:
```bash
gcc -Wall -Wextra main.c -o inventory_system
```

### Execution

```bash
./inventory_system
```

On Windows:
```bash
inventory_system.exe
```

---

## 📖 Usage Guide

### Main Menu Options

1. **Load Data** - Loads products, branches, and inventory from files
2. **Search Product** - Search by code or description
3. **Shopping Cart** - Manage cart items
   - Add item
   - Remove item
   - List cart contents
   - Remove specific quantity
4. **Check Availability** - Verify which branches can fulfill the order
5. **Choose Branch & Finalize** - Complete purchase at selected branch
6. **Load Cart from File** - Process batch commands
0. **Exit** - Clean up memory and exit

### Example Workflow

```
1. Load Data (Option 1)
2. Add items to cart (Option 3 → 1)
3. Check which branches can fulfill order (Option 4)
4. Finalize purchase at chosen branch (Option 5)
```

---

## 🔍 Key Features Explained

### Smart Cart Adjustment
If a branch cannot fulfill 100% of the order, the system automatically adjusts the cart to match available stock, ensuring the purchase can proceed.

### Stock Management
- Real-time stock verification
- Automatic stock deduction after purchase
- Updated inventory saved to file

### Transaction History
- Each purchase generates a receipt (`ultima_compra.txt`)
- All transactions logged in `relatorio_geral.txt`

---

## 💾 Memory Management

The program properly manages dynamic memory with:
- Allocation using `malloc()`
- Deallocation using `free()`
- Cleanup functions for all data structures

All memory is freed before program termination.

---

## 🐛 Known Limitations

- Product descriptions must be single words (no spaces)
- Branch names and cities must be single words
- Maximum description length: 40 characters
- Maximum name/city length: 50 characters

---

## 📝 Sample Data

Create your own test files or use these examples:

**produtos_v2.txt:**
```
101 Aspirin 12.50
102 Ibuprofen 15.75
103 Bandages 8.00
104 Vitamins 28.90
105 Thermometer 45.00
```

**filiais_v2.txt:**
```
1 Downtown Chicago
2 Suburbs Houston
3 Mall Miami
```

**estoques_v2.txt:**
```
1 101 100
1 102 50
1 103 200
2 101 30
2 104 80
3 102 60
3 105 25
```

---

## 🤝 Contributing

This is an academic project, but suggestions and improvements are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Commit changes (`git commit -m 'Add improvement'`)
4. Push to branch (`git push origin feature/improvement`)
5. Open a Pull Request

---

## 📜 License

This project is available for educational purposes. Feel free to use and modify as needed.

---

## 📧 Contact

For questions or suggestions, feel free to reach out to the development team.

---

## 🎓 Academic Context

This project was developed as part of a Data Structures course, demonstrating practical implementation of:
- Linked lists
- File processing
- Dynamic memory management
- Modular programming in C
