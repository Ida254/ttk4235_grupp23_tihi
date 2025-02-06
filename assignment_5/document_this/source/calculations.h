/**
 * @file
 * @brief A simple attempt at documenting the file 'calculations.h'.
 */
#ifndef CALCULATIONS_H
#define CALCUATIONS_H
#include "system.h"

/**
 * @brief Calculates the P parameter for the given system.
 *
 * @param[in] current_system A pointer to a 'System' structure containing
 *                          the necessary data to perform the calculation.
 *                          It is assumed that the system is already initialized
 *                          with the required values. The parameter is **input only**
 *
 * @return The calculated P parameter as a 'double' value.
 *          The value represents the result of the calculation.
 */
double calculate_P_parameter(System *current_system);

/**
 * @brief Calculates the I parameter for the given system.
 *
 * @param[in] current_system A pointer to a 'System' structure
 *                          @p current_system containing
 *                          the necessary data to perform the calculation.
 *                          It is assumed that the system is already initialized
 *                          with the required values. The parameter is **input only**
 *
 * @return The calculated I parameter as a 'double' value.
 *          The value represents the result of the calculation.
 */
double calculate_I_parameter(System *current_system);

/**
 * @brief Calculates the D parameter for the given system.
 *
 * @param[in] current_system A pointer to a 'System' structure
 *                          @p current_system containing
 *                          the necessary data to perform the calculation.
 *                          It is assumed that the system is already initialized
 *                          with the required values. The parameter is **input only**
 *
 * @return The calculated D parameter as a 'double' value.
 *          The value represents the result of the calculation.
 */
double calculate_D_parameter(System *current_system);

#endif
