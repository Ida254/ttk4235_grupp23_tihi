# Doxygen
Just an intro to Doxygen ... for those who forgets easily.

- [Doxygen](#doxygen)
  - [Configuration](#configuration)
  - [Syntax and Commenting Code](#syntax-and-commenting-code)
    - [NB! Always Include](#nb-always-include)
    - [Commenting Functions](#commenting-functions)
  - [Example](#example)

## Configuration
Before running Doxygen it is important to make a configurationfile. This is done by navigating to the folder *above* the file that you want to document, and run the command

```bash
doxygen -g doxconfig
```

This file decides the different paramters and settings for the project. You could for example changes these parameters

```bash
PROJECT_NAME = "Memory Library Example"
OPTIMIZE_OUTPUT_FOR_C = YES
INPUT = source/
SOURCE_BROWSER = YES
```

## Syntax and Commenting Code
To start commands, use `@`.
* `@brief` - a short description of what a function do
* `@param` - a short description of the task of a function paramater
    * It is also common to add `[in]`, `[out]` or `[in,out]`. This is not necessary, but tells us if the parameter is modefied by the function or not.
    * Using `[in]`, `[out]` and `[in,out]` is excpesially usefull when using pointers in `C/C++`.
    * `[in]` - *input*, the function reads from the parameter, but do not change it's value
    * `[out]` - *output*, the function changes the parameter outside of the function, but do not read from it directly, commen when using pointers
    * `[in,out]` - *input* and *output*, read from the parameter, and changes it outside of the function
* `@return` - documents the return value of the function, not needed for `void`-functions
* `@warning` - this section wars the user about potential issues
* `@c` - renders text as inline code in **monospace font**. Emphasize **any code** or identifier
* `@p` - formates the names of parameters and variables. Similar to `@c`, but used specifically for parameters

### NB! Always Include
The first Doxygen do is look through the code after a `@file`-command. This should be at the top of the file (no `@file` at the top of the code results in an empty documentation) and communicates to Doxygen that it should be generated documentation for this file.
```bash
/**
* @file
* @brief [description]
*/
```

### Commenting Functions
When commenting functions, start with

```bash
/**
* @param[in] source buffer
* @param[out] destination buffer
* @param[in,out] size Number of integers in the buffer
*
* @return what it expects to return
*/
```

## Example
```bash
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