# Heislab
Just some basics about how to initialize this project. This is mostly from the guide given out with the assignment.

- [Heislab](#heislab)
  - [Heissimulator](#heissimulator)
    - [How to set up the `heissimulator`](#how-to-set-up-the-heissimulator)
    - [GUI and buttons](#gui-and-buttons)
- [Set working pc up with github](#set-working-pc-up-with-github)

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

# Set working pc up with github
Before starting, move into the git repository.

* Location of the `id_ed25519` on the pc at lab is:
  ```bash
  ~/Desktop/gruppe23_tihi/.ssh/id_ed25519
  ```

* Ensure that the SSH Key is loaded
  ```bash
    ssh-add ~/Desktop/gruppe23_tihi/.ssh/id_ed25519
  ```

  * If you encounter an error try
    ```bash
    eval "$(ssh-agent -s)"
    ssh-add ~/Desktop/gruppe23_tihi/.ssh/id_ed25519
    ```

* How to copy the SSH Key:
    ```bash
    ~/Desktop/gruppe23_tihi/.ssh/id_ed25519
    ```

* How to verify the key:
  ```bash
  ssh -T git@github.com
  ```