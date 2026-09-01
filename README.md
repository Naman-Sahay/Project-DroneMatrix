# Smart Drone Delivery Management System

An automated UAV fleet scheduling and route verification system developed in C++ to prevent mid-air battery exhaustion and optimize urban last-mile delivery.

---

## 📌 Project Overview
Traditional drone dispatch models often select the nearest available drone without verifying full round-trip route feasibility. This leads to drones depleting their battery reserves mid-flight on their return journey back to charging hubs. 

This project implements an intelligent scheduling engine that prioritizes incoming customer orders, tracks live fleet telemetry, validates battery energy demands for the entire circuit with a mandatory safety buffer, and dispatches the optimal drone using a multi-factor suitability scoring algorithm.

---

## 🚀 Key Features
* **Priority Order Queueing:** Uses a min-heap (`std::priority_queue`) to rank customer delivery requests based on package urgency and customer wait times.
* **Live Fleet Telemetry:** Tracks real-time GPS locations, battery percentages, and operational states (`AVAILABLE`, `BUSY`, `CHARGING`) using an $O(1)$ lookup hash map (`std::unordered_map`).
* **Round-Trip Battery Gate:** Verifies energy feasibility for the complete trip ($\text{Hub} \rightarrow \text{Pickup} \rightarrow \text{Customer} \rightarrow \text{Hub}$) with a mandatory **15% safety buffer**.
* **Suitability Scoring Engine:** Ranks safe candidate drones using a weighted scoring formula ($S = w_1 \cdot d_{\text{pickup}} + w_2 \cdot \Delta E + w_3 \cdot t_{\text{wait}}$) to dispatch the best unit.
* **Automated Charging Loop:** Routes low-battery drones directly to automated hub charging and battery swapping bays.

---

## 🛠️ Technology Stack
* **Language:** C++ (C++17 / C++20)
* **Data Structures (STL):** `std::priority_queue`, `std::unordered_map`, `std::vector`
* **Build Tools:** GCC / Clang / CMake
* **Documentation & Typesetting:** LaTeX, TikZ

---

## 👥 Team DroneMatrix
* **Team ID:** DSCPP-III-2026-T133
* **Course:** B.Tech (CSE), 3rd Semester
* **Mentor:** Dr. Hemant Singh Pokhariya

| Member | Roll Number | Role |
| :--- | :--- | :--- |
| **Suryansh Jaiswal (Lead)** | 2028200 | Overall system flow & finding the best drone logic |
| **Naman Sahay** | 2028776 | Battery safety checker & flight distance calculator |
| **Nitin Singh** | 2027950 | Order priority queue & live drone tracking maps |
| **Hemang Bhai Patel** | 2027809 | City delivery simulation & project testing |

---

## 📄 Project Documentation
* [Phase-1 Proposal Report (PDF)](./Report/Phase-1.pdf)
* [Phase-1 LaTeX Source](./Report/Phase-1.tex)
