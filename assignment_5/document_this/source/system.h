/**
 * @file
 * @brief A simple file about creating and deleting the system.
 */
#ifndef SYSTEM_H
#define SYSTEM_H

typedef enum
{
    Classic_Ziegler_Nichols,
    Pessen_Integral_Rule
} Tuning;

/**
 * @brief A structure to represent a system and desired PID tuning method.
 */
typedef struct
{
    double Ku;            /** < The gain margin for loop stability */
    double Tu;            /** < The period of the oscillations frequency at the stability limit */
    Tuning tuning_method; /** < The desired PID tuning method */
} System;

/**
 * @brief Creates and initializes a new System.
 *
 * This function creates a new `System` structure, initializing it with the given
 * values of Ku, Tu, and the tuning method. The returned `System` pointer
 * can then be used for further processing or control.
 *
 * @param[in] Ku The ultimate gain parameter used in the system creation.
 *               It is a tuning parameter that affects the system's behavior.
 * @param[in] Tu The ultimate period used in the system creation.
 *               It is another tuning parameter that helps define system dynamics.
 * @param[in] tuning_method The tuning method to be used when creating the system.
 *                          This parameter determines how the system's values are set.
 *
 * @return A pointer to the newly created and initialized `System` structure.
 *         The caller is responsible for managing the memory of the returned structure.
 */
System *system_create(double Ku, double Tu, Tuning tuning_method);

/**
 * @brif Deletes the system for the given system.
 *
 * @param[in] current_system A pointer to a 'System' structure that is to be deleted.
 */
void system_delete(System *system_current);

#endif
