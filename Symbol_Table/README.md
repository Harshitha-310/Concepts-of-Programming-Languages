<h1> 🧠 Symbol Table Implementation in C </h1>
<p>This project implements a "hierarchical symbol table" (scope-based) in C using hash tables and chaining. It supports variable declarations across nested scopes and allows efficient lookup with proper shadowing resolution.</p>
<br>
<h2> ✨ Features </h2>
<ul>
    <li>🔍 Fast string-based symbol lookup using "DJB2 hashing"</li>  
    <li>📚 "Scoped symbol table" — simulates nested environments </li>
    <li>🔁 "Shadowing support" — inner scopes override outer ones  </li>
    <li>🗑️ Proper "memory cleanup" to avoid memory leaks  </li>
    <li>📦 Collision handling using "chaining (linked list)"  </li>
</ul>
<br>
<h2>⚙️ Compilation & Execution</h2>
<h3> 🧰 GCC (Linux/macOS/WSL/MinGW):</h3>
<ul>
    <li>gcc symbol_table.c -o symbol_table</li>
    <li>./symbol_table</li>
</ul>
<br>
<h3> 🧪 Output </h3>
<p>
    2
    2
    3
    5
    3
    4
    1
</p>

