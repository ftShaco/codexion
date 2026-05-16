# 🧠 Codexion

> A high-performance multithreading and concurrency simulation written in C, inspired by the Dining Philosophers problem — reimagined for modern software engineering workflows.

<p align="center">
  <img src="https://img.shields.io/badge/language-C-blue.svg" />
  <img src="https://img.shields.io/badge/threading-POSIX%20Threads-success.svg" />
  <img src="https://img.shields.io/badge/synchronization-EDF%20%7C%20FIFO-orange.svg" />
  <img src="https://img.shields.io/badge/status-42%20Project-9cf.svg" />
</p>

---

## 📚 About

This project has been created as part of the **42 curriculum** by **[Your Name]**.

**Codexion** simulates a team of coders attempting to:
- 🛠 Compile
- 🐞 Debug
- ♻️ Refactor

...while competing for limited shared resources called **dongles**.

The project focuses on mastering:
- Thread synchronization
- Deadlock prevention
- Starvation avoidance
- Precise timing systems
- Real-time scheduling algorithms

Unlike traditional implementations, Codexion introduces:
- Advanced scheduling (`FIFO` & `EDF`)
- Resource cooldown systems
- Smart sleeping mechanisms
- Precise burnout detection

---

# ⚙️ Compilation

Compile the project from the root directory:

```bash
make