# Heislab
Just some basics about how to initialize this project. This is mostly from the guide given out with the assignment.

- [Heislab](#heislab)
  - [GUI and buttons](#gui-and-buttons)
  - [Run the elevator at the lab](#run-the-elevator-at-the-lab)
  - [Elevatorsimulator ('heissimulator')](#elevatorsimulator-heissimulator)
    - [How to set up the `heissimulator`](#how-to-set-up-the-heissimulator)
- [Set working pc up with github](#set-working-pc-up-with-github)

## GUI and buttons ##
- `#` - the elevator is right above the `Floor`

- `#>` - the elevator is moving upwards

- `<#` - the elevator is moving downwards

- `qwe` - `up` buttons

- `sdf` - `down` buttons

- `zxcv` - floor buttons inside the elevator

- `-` - `obstruction` button

- `p` - `stop` button

## Run the elevator at the lab ##
To make the elevator run at the lab:
1. Check if the everything is set up correctly
   * Turn *on* the PC
   * Make sure everything is up to date
   * Toggle to `pc` and `obstruction` on the elevator panel
2. On the PC, open the terminal and go to `heislab/skeleton_project`
3. Run
   ```bash
   make
   elevatorserver
   ```
4. Open a new elevator, makeing sure you are in the correct folder (`heislab/skeleton_project`), and run
   ```bash
   make
   ./elevator
   ```
5. Now the elevator should run

## Elevatorsimulator ('heissimulator') ##

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