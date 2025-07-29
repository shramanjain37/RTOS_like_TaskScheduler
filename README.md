# Minimal RTOS-like Task Scheduler + Secure Logger

## Overview

This project implements a **bare-metal cooperative task scheduler** with integrated **secure logging** on an STM32 Nucleo-C071RB microcontroller. It mimics core RTOS functionality (task scheduling, context separation, non-blocking delays) while maintaining full control over system behavior and performance—ideal for deterministic embedded systems with high integrity requirements. This project has 2 parts: Round-robin scheduling and Priority-based scheduling (without preemption).

---

## 📌 Features

- **Cooperative Task Scheduler** (no RTOS dependency)
  - Lightweight task structure
  - Round-robin execution
  - Priority-based execution
  - Software timer-based task delays (non-blocking)
- **Secure Logging Module**
  - Log messages/events with timestamps
  - CRC32 integrity protection for each log entry
  - Flash-backed persistent storage
- **Efficient & Predictable**
  - No dynamic memory
  - Designed for interrupt-driven I/O

---



