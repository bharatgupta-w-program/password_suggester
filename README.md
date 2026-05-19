# password_suggester
# Stateless Deterministic Password Generator (Gram-Schmidt Based)

A stateless, zero-storage command-line tool written in C++. This project bridges the gap between **Linear Algebra** and **Software Design** by utilizing vector orthogonalization to derive unique, deterministic passwords from a username and master password pairing.

---

## 💡 The Core Concept

Traditional password managers store your credentials in a central encrypted database. If the database is compromised, your data is at risk. 

This project explores a **stateless architecture** requiring zero storage. It treats your inputs as vectors in a 32-dimensional Euclidean space ($\mathbb{R}^{32}$):
1. The **Username** is converted into a vector $u$.
2. The **Master Password** is converted into a vector $m$.
3. The program applies the **Gram-Schmidt process** to isolate a unit vector $e_2$ that is strictly orthogonal (perpendicular) to $u$.
4. The components of $e_2$ are mapped onto a custom character set to produce a strong password.

Because the math is entirely deterministic, entering the same inputs will always generate the exact same password, eliminating the need to save any data on a disk.

---

## 🛠️ How It Works (The Math)

The program maps strings to 32-dimensional vectors and extracts an independent, orthogonal basis vector using the following steps:

1. **Trigonometric Matrix Padding:** Instead of using simple modulo loop padding for short strings—which leaks input length structures—the inputs are passed through a weighted trigonometric distribution function. Every character in the input string acts as a distinct frequency component that affects all 32 dimensions of the vector space simultaneously:
   $$v[i] = \sum_{j} s[j] \cdot \sin((i+1) \cdot (j+1) \cdot 0.31415926)$$

2. **Vector Projection:** The master password vector $m$ is projected onto the normalized username unit vector $e_1$:
   $$c_1 = m \cdot e_1$$

3. **Orthogonalization:** The parallel component is subtracted from $m$ to find the orthogonal vector $w$, which is then normalized to get the unit vector $e_2$:
   $$w = m - c_1 e_1 \quad \implies \quad e_2 = \frac{w}{\|w\|}$$

4. **Non-Parametric Rank Mapping (`argsort`):** Because orthogonal unit vectors exhibit tight mathematical variance clustering around zero, traditional range-mapping heavily favors characters in the middle of a charset pool. This project resolves this by sorting and tracking the relative magnitude *rank* ($0$ to $31$) of each vector component to distribute selections perfectly evenly across a 76-character set containing uppercase, lowercase, numbers, and symbols.

---

## ⚠️ Cryptographic Limitations: "Don't Roll Your Own Crypto" 🚫🔑

This repository is strictly an **educational toy project** exploring linear algebra applications. When analyzed through a professional cybersecurity lens, several constraints emerge that explain why production-grade security standards (like PBKDF2 or Argon2) rely on bitwise transformations rather than pure geometry:

* **Linear Predictability:** Gram-Schmidt is an entirely linear transformation. Purely linear systems can theoretically be modeled and broken using constraint-satisfaction mathematical solvers if an attacker intercepts enough output matching pairs. Production cryptography requires *confusion and diffusion* via non-linear bitwise substitution boxes (S-boxes).
* **Floating-Point Cross-Platform Dependence:** The code relies on continuous `double` floating-point calculations and the `<cmath>` `sin()` function. Tiny rounding variations at the 15th decimal place between different hardware architectures (e.g., Intel x86 vs. ARM) or compiler optimization flags could alter the `argsort` sequence, generating a completely different password and locking the user out. Real crypto relies strictly on stable integer math.

---

