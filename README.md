# HaikuMCPClient for LLM/MCP Interaction

## 🌟 Overview
The **HaikuMCPClient** is a dedicated graphical client designed for interacting with Large Language Models (LLMs) and the Model Context Protocol (MCP). This application provides an intuitive interface on top of the specialized AI subsystem integrated into the Haiku distribution, known as **Pirati Del Frico**. It allows users to easily manage complex interactions between various AI services.

## ⚙️ Architecture
The core functionality is built using C++ and it uses the AI Kit provided by *Pirati Del Frico* ai subsystem

## 🚀 Compiling and Running
To compile this client, please ensure you have a standard C++ development environment installed on Haiku.

1.  **Navigate to the directory:**
    ```bash
    cd /boot/home/Apps/HMCP
    ```

2.  **Compile the project using Make:**
    The `Makefile` handles linking all necessary components into the executable (`HaikuMCPClient`).
    ```bash
    make
    ```
    *(Note: The Makefile should compile `hmcp.cpp`, `MainWindow.cpp`, and any dependencies listed).*

3.  **Run the client:**
    Once compilation is successful, you can launch the application directly:
    ```bash
    ./HaikuMCPClient
    ```

## ⚠️ Dependencies
This client runs specifically on top of the **Pirati Del Frico** subsystem—a sophisticated and integrated AI layer built into Haiku. Proper functioning requires that the core services provided by Pirati Del Frico are running and accessible to the application environment.

***
*Developed for the Haiku derived operating system called Pirati Del Frico.*