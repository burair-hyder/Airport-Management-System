# ✈️ Airport Management System – C++ OOP Project
## —*🛫*no turbulence, just classes, objects, and perfectly handled exceptions.*😎*



This is a **console-based Airport Management System** built using Object-Oriented Programming (OOP) in C++. The system allows administrators to manage flights, passengers, cargo, and staff. It supports real-time feedback recording, flight analytics, and file persistence, making it a comprehensive solution for basic airport operations in an academic context.

---


**Group Members:**
- 🎓 Mutahir Ahmed – 24K-0030  
- 🎓 Burair Hyder – 24K-0804  
- 🎓 Sameed Imran – 24K-1036  
- 🎓 Ammar Kamran – 24K-0732


## 🚀 Features

- 🛫 Add, view, and manage **flights**
- 👤 Add and remove **passengers** from flights
- 📦 Manage **cargo** assignments to flights
- 🧑‍💼 Add and view **staff** information
- ⏰ Mark flights as **delayed** with updated departure times
- 💬 Collect and display **passenger feedback**
- 📊 Generate **analytics reports** for popular destinations
- 🔍 **Compare flights** based on destination and duration
- 💾 Save/load all data using `.txt` files
- 🛡️ Simple **admin login system**
- ⚠️ Robust **exception handling** for file I/O

---

## 🔧 Technologies Used

- C++  
- Standard Template Library (`<vector>`, `<fstream>`, `<exception>`, etc.)
- File I/O
- Console UI with `iomanip` formatting

---

## 📁 File Structure

- `main.cpp` – contains the full implementation
- `flights.txt` – stores flight data
- `passengers.txt` – stores passenger data
- `cargo.txt` – stores cargo data
- `staff.txt` – stores staff data

These `.txt` files are auto-generated and updated during program execution.

---

## 🛠️ How to Compile and Run

### 🖥️ Requirements
-( *PLIS* *PLIS* first *create* the above mentioned files in the project folder. 😌)
<br>
-(the system does not create them on its *OWN*.)

- Username : admin
- password = 1234
- C++ Compiler (e.g., g++, clang++, or Dev-C++ for Windows)

### ▶️ Compile
```bash
g++ main.cpp -o airport_system
