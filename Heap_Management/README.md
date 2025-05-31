<h1> 🧠 Heap Memory Management using Buddy System in C </h1>
<p>This project demonstrates a "heap memory manager" implemented in C using the "Buddy Memory Allocation" algorithm. It supports dynamic allocation and deallocation of memory blocks, merging and splitting them as needed for efficient memory use.</p>
<br>
<h2>✨ Features </h2>
<ul>
    <li>🚀 Implements the "Buddy Memory Allocation" strategy </li> 
    <li>🧩 Splits large blocks to fit requested size </li> 
    <li>🔁 Merges free buddies during deallocation </li> 
    <li>📏 Handles memory block sizes from "16B to 1024B" </li>
    <li>🖥️ Interactive "CLI Menu" for allocation/deallocation </li>
</ul>
<br>
<h2> ⚙️ Compilation & Execution </h2>
<h3> 🧰 GCC (Linux/macOS/WSL): </h3>
<ul>
    <li>gcc Heap_management.c -o heap</li>
    <li>./heap</li>
</ul>
<br>
<h2> 🧪 Sample Run </h2>
<h4>Buddy System Memory Management</h4>
<ul>
    <li>1. Allocate Memory </li>
    <li>2. Free Memory </li>
    <li>3. Exit </li>
<ul>
<h5>Enter your choice: 1 </h5>
<ul>
    <li>Enter the size of memory to allocate: 100 </li>
    <li>Allocated 128 bytes at address 0x55c4a8ff7260 </li>
</ul>
<h5>Enter your choice: 2 </h5>
<h6>Allocated addresses: </h6>
<ul>
    <li>1. 0x55c4a8ff7260 </li>
    <li>Enter index of address to free: 1 </li>
    <li>Freed memory at address 0x55c4a8ff7260 </li>
</ul>


