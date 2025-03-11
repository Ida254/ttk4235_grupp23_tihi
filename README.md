# ttk4235_grupp23_tihi

Working folder for the subject TTK4235 (spring 2025).

<details>
  <summary>📖 Table of Contents</summary>

- [ttk4235_grupp23_tihi](#ttk4235_grupp23_tihi)
- [Set working pc up with github](#set-working-pc-up-with-github)

</details>

# Set working pc up with github

Before starting, move into the git repository.

1. Location of the `id_ed25519` on the pc at lab is `~/Desktop/gruppe23_tihi/.ssh/id_ed25519`

   ```bash
   cd ~/Desktop/gruppe23_tihi/
   ```

2. Ensure that the SSH Key is loaded

   ```bash
     ssh-add ~/Desktop/gruppe23_tihi/.ssh/id_ed25519
   ```

   - If you encounter an error try
     ```bash
     eval "$(ssh-agent -s)"
     ssh-add ~/Desktop/gruppe23_tihi/.ssh/id_ed25519
     ```
   - How to copy the SSH Key:
     ```bash
     cat ~/Desktop/gruppe23_tihi/.ssh/id_ed25519.pub
     ```

3. How to verify the key:
   ```bash
   ssh -T git@github.com
   ```
