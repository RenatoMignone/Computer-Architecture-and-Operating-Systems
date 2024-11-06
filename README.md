# Computer Architectures and Operating Systems

This repository contains materials and code for the **Computer Architectures and Operating Systems** course. In this course, we explore the foundational principles of computer architecture and operating systems, with an emphasis on real-time embedded systems.

## Course Overview

- **Computer Architectures**: This part of the course covers the internal structure and operation of computer systems, including CPU design, memory hierarchies, I/O systems, and more.
- **Operating Systems**: We study various types of operating systems, their components, and how they manage hardware resources. A key focus is on real-time operating systems (RTOS).

In this repository, we focus on implementing and experimenting with **FreeRTOS**, a real-time operating system that is widely used in embedded systems.

## Repository Structure

The repository is organized into two main folders and a submodule for the FreeRTOS kernel:

### 1. `Exercises/`
This folder contains the exercises made in class with the professor and someone made by me.

### 2. `Laboratories/`
This folder contains a series of laboratories made during the course.

### 3. `FreeRTOS/` (Submodule)
This submodule contains the kernel of **FreeRTOS**, which we are using as a real-time operating system in this course. FreeRTOS is an open-source RTOS designed for embedded systems and microcontrollers. It provides features like multi-threading, task synchronization, and real-time scheduling.

To initialize the submodule and fetch the FreeRTOS code, run:

```bash
git submodule init
git submodule update
