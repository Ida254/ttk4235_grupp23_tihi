# Doxygen: Introduction Guide
Doxygen is a tool used for generating documentation from annotated source code. It is widely used for creating detailed documentation for C, C++, and other programming languages. Doxygen can automatically extract documentation from your code, so you don't have to manually write it all out. This README will guide you through how to set up and use Doxygen for your projects.

- [Doxygen: Introduction Guide](#doxygen-introduction-guide)
  - [Configuration](#configuration)
    - [1. Generate a Configuration File](#1-generate-a-configuration-file)
    - [2. Edit the Configuration file](#2-edit-the-configuration-file)
    - [3. Running Doxygen](#3-running-doxygen)
  - [Syntax and Commenting Code](#syntax-and-commenting-code)
    - [NB! Always Include](#nb-always-include)
    - [Commenting Functions](#commenting-functions)
  - [Tips](#tips)
    - [Best Practices](#best-practices)
    - [Troubleshooting](#troubleshooting)
  - [Example](#example)

## Configuration

Before running Doxygen, you need to create a configuration file. This file defines various parameters and settings for how Doxygen generates the documentation. Here's how to set it up:

### 1. Generate a Configuration File

Navigate to the folder *above* the files you want to document and run the following command:

```bash
doxygen -g doxconfig
```

This generates a file called `doxconfig` (or whatever you choose to name it). You can then edit this configuration file to suit your needs.

### 2. Edit the Configuration file

Open the configuration file in a text editor:

```bash
nano doxconfig
```

or use `VScode`

```bash
code doxconfig
```

Make changes to the configuration file. For example:

```bash
PROJECT_NAME = "Memory Library Example"
OPTIMIZE_OUTPUT_FOR_C = YES
INPUT = source/
SOURCE_BROWSER = YES
```

If your files are in subfolders, set the following to make Doxygen recurse into subdirectories:

```bash
RECURSIVE = YES

```

### 3. Running Doxygen
After commenting the code, run Doxygen using:

```bash
doxygen doxconfig
```

## Syntax and Commenting Code
Doxygen uses special commands, most of which start with ``@``. Here's a quick guide to some of the most commonly used Doxygen commands:
* `@brief` - A short description of what a function does
* `@param` - Describes a function's parameters
    * You can add `[in]`, `[out]` or `[in,out]` to clarify whether the paramater is input, output, or both.
    * `[in]` - The function reads the paramater but does not change its value.
    * `[out]` - The function modifies the paramater
    * `[in,out]` - The function both read from and modifies the paramater
* `@return` - Describes the return value of a funciton (not needed for `void` functions).
* `@warning` - Used to highlight potential issues or side effects.
* `@c` - Formats text as inline code (monospace font).
* `@p` - Specifically formats paramaters names and variables, similar to `@c`.

### NB! Always Include
To make sure Doxygen processes your file, always start with the `@file` command at the top of your source code:
```c
/**
* @file
* @brief [description]
*/
```

This tells Doxygen that it should generate documentation for this file.

### Commenting Functions
When commenting functions, use the following structure:

```c
/**
 * @brief [brief description of the function]
 *
 * @param[in] source Buffer of integers to be multiplied.
 * @param[out] destination Buffer to hold the result.
 * @param[in,out] size Number of integers in the buffer.
 *
 * @return What the function returns (if applicable).
 */
```

This is a typical function comment format that helps generate clear, understandable documentation.

## Tips

This section provides helpful hints to make your Doxygen documentation more effective and efficient. By following these tips, you can ensure that your code comments are clear, concise, and useful for both developers and documentation users. Whether you're new to Doxygen or a seasoned user, these tips will improve your workflow and the overall quality of your generated documentation.

### Best Practices
* Always keep your comments concise and to the point.
* Use `@brief` for a one-liner description of what a function or file does.
* Make use of `@param[in]`, `@param[out]`, and `@param[in,out]` to clarify the roles of parameters, especially when pointers are involved.
* Include `@warning` or `@note` whenever there is a potential risk or important note about the code's behavior.

### Troubleshooting
* **Doxygen Doesn’t Generate Documentation:**:
  Ensure you have the @file tag at the top of each source file.
  Check if the INPUT path in your doxconfig points to the correct directories.

* **Recursive Option Not Working:**
  Ensure you set `RECURSIVE = YES` in your configuration if your files are in subfolders.

  ## Example

Here’s an example function comment for a multiplication function:

```c
/**
* @brief Multiply all the elements in @p p_buffer, of size
* @p size with the supplied @p factor.
*
* @param[in,out] p_buffer Buffer of integers to be multiplied
* with @p factor.
*
* @param[in] factor Factor to multiply each of the
* elements in @p p_buffer with.
*
* @param[in] size Size of @p p_buffer.
*
* @warning If @p p_buffer is @c NULL, the function will
* abruptly terminate the program with exit code @c 1.
*/
```