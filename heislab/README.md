# Heislab
Just some basics about how to initialize this project. This is mostly from the guide given out with the assignment.

## Heissimulator ##

### How to set up the `heissimulator` ###
To initialize the `heissimulator`, do as follows:
1. open the terminal and the `skeleton_project` folder

2. run the command:
    ```bash
    chmod +x SimElevatorServer
    ```
    to make it possible to run the simulator as a program. This only needs to be done once.

3. run the command
    ```bash
    ./SimElevatorServer
    ```
    in the terminal to start the simulation

4. open another terminal in the `skeleton_project` folder

5. compile the elevator program in the new terminal as usual with
    ```bash
    make
    ./elevator
    ```

### GUI and buttons ###
- `#` - the elevator is right above the `Floor`

- `#>` - the elevator is moving upwards

- `<#` - the elevator is moving downwards

- `qwe` - `up` buttons

- `sdf` - `down` buttons

- `zxcv` - floor buttons inside the elevator

- `-` - `obstruction` button

- `p` - `stop` button