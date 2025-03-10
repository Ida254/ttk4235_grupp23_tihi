#include "utilities.h"

const char *button_to_string(ButtonType button)
{
    switch (button)
    {
    case BUTTON_HALL_UP:
        return "B_UP";
    case BUTTON_HALL_DOWN:
        return "B_DOWN";
    case BUTTON_CAB:
        return "B_CAB";
    default:
        return "UNKNOWN";
    }
}

const char *motor_direction_to_string(MotorDirection direction)
{
    switch (direction)
    {
    case DIRN_UP:
        return "D_UP";
    case DIRN_DOWN:
        return "D_DOWN";
    case DIRN_STOP:
        return "D_STOP";
    default:
        return "UNKNOWN";
    }
}

const char *bool_to_string(bool true_or_false)
{
    if (true_or_false)
    {
        return "TRUE";
    }
    else if (!true_or_false)
        return "FALSE";
    return "UNKNOWN";
}

ButtonType int_to_button_type(int integer)
{
    ButtonType button;
    switch (integer)
    {
    case 0:
        button = BUTTON_HALL_UP;
        break;
    case 1:
        button = BUTTON_HALL_DOWN;
        break;
    default:
        button = BUTTON_CAB;
        break;
    }
    return button;
}

MotorDirection int_to_motor_direction(int integer)
{
    MotorDirection motorDir;
    switch (integer)
    {
    case -1:
        motorDir = DIRN_DOWN;
        break;
    case 1:
        motorDir = DIRN_UP;
        break;
    default:
        motorDir = DIRN_STOP;
        break;
    }
    return motorDir;
}

MotorDirection button_type_to_motor_direction(ButtonType btn)
{
    MotorDirection motorDir;
    switch (btn)
    {
    case BUTTON_HALL_UP:
        motorDir = DIRN_UP;
        break;
    case BUTTON_HALL_DOWN:
        motorDir = DIRN_DOWN;
        break;
    default:
        motorDir = DIRN_STOP;
        break;
    }
    return motorDir;
}