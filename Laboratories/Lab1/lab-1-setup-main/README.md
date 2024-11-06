# Lab 1: Setup

## Introduction

In this lab, you'll set up a QEMU (Quick Emulator) environment for ARM Cortex-M development. QEMU is
a generic and open-source machine emulator and virtualizer. It can emulate various CPUs and boards,
including ARM-based ones. You'll use QEMU to run a simple ARM Cortex-M program on a NetduinoPlus2
development.

You will learn how to: set up a QEMU environment for ARM Cortex-M development, how to compile
programs for ARM Cortex-M using the GNU toolchain, how to run programs on QEMU in User Mode
Emulation and System Mode Emulation to finally debug the program using GDB.

GDB is a powerful debugger that supports various programming languages, including C, C++, and
Assembly. You'll use GDB to debug the ARM Cortex-M program running on QEMU.

## Prerequisites

To complete this lab, you'll need a machine with a Linux-based operating system. You can use either
a virtual machine or a dual-boot setup. The following instructions assume you're using a
Debian-based distribution such as Ubuntu.

## Getting Started

Follow these steps to set up your environment:

**Update the package list and upgrade the system:**
    Open a terminal and run the following commands to ensure your system is up to date:
  ```bash
  sudo apt update
  sudo apt upgrade
  ```

**Install the required packages:**

  - **Build-essential package:**
  The `build-essential` package provides a collection of tools required for compiling and building
  software, including the GNU Compiler Collection (GCC) and GNU Make. Install it by running:

  ```bash
  sudo apt install build-essential
  ```

  - **Git:**
  `git` is a distributed version control system that you'll use to clone, push, and pull
  repositories throughout this course. Install Git by running:

  ```bash
  sudo apt install git
  ```

**Clone the Lab Repository:**
  Once Git is installed, you can clone the repository that contains all the necessary files for this
  lab. Make sure you've registered with the GitLab repository before attempting to clone it:

  ```bash
  git clone <repository-url>
  ```

## Lab Exercises
  After completing the setup, proceed with the following exercises:
  - [Exercise 1: User Emulation](./Ex1_UserEmulation/README.md)
  - [Exercise 2: Bare-metal System](./Ex2_Baremetal/README.md)
  - [Exercise 3: GDB Debugging](./Ex3_GDB/README.md)


