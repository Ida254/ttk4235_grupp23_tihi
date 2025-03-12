# Git

<details>
 <summary>📖 Table of Contents</summary>
 
- [Git](#git)
  - [💻 Commands](#-commands)
  - [🔑 How to make SSH key](#-how-to-make-ssh-key)
    - [🛠️ This is a tutorial for how to make a `SSH key`](#️-this-is-a-tutorial-for-how-to-make-a-ssh-key)
    - [🎯 Summary of Commands](#-summary-of-commands)
    - [How to Add SSH Key to a Custom Location](#how-to-add-ssh-key-to-a-custom-location)
  - [📝 How to Create and Use a `.gitignore` File](#-how-to-create-and-use-a-gitignore-file)
</details>


## 💻 Commands
1. `git status` - Shows the current status of your working directory.
     
      ```bash
      git status
      ```

2. `git add <file>` - Stages changes for commit.
 
      ```bash
      git add <file>
      ```

      To add all changes: `git add .`

3. `git commit -m "<message>"` - Commits changes with a message.
      ```bash
      git commit -m "Commit message here"
      ```
  
4. `git log` - Displays the commit history.
      ```bash
      git log
      ```

      **Common flags**:
    * `git log --online` (shows a brief commit history)
    * `git log --graph` (shows commit history as a graph)

5. `git checkout <branch>` - Switch to a different branch.
   ```bash
   git checkout <branch>
   ```

   Create and switch to a new branch: `git checkout -b new-branch`
  
6. `git branch` - List or manage branches.


    ```bash
    git branch          # List all branches
    git branch <name>   # Create a new branch
    git branch -d <name>  # Delete a branch
    ```

    **Common flags**:
   * `git log --online` (shows a brief commit history)
   * `git log --graph` (shows commit history as a graph)


7. `git pull` - Fetches and merges changes from a remote repository.

    ```bash
    git pull origin main
    ```

8. `git push` - Pushes committed changes to a remote repository.

    ```bash
    git push origin branch_name
    ```

9.  `git fetch` - Fetches changes from a remote repository without merging.

    ```bash
    git fetch origin
    ```

10. `git merge <branch>` - Merges changes from one branch into another.

    ```bash
    git merge branch_name

    ```

11. `git reset` - Undo commits or unstage changes.

    ```bash
    git reset <commit>    # Unstage files or move HEAD to a commit
    git reset --hard     # Completely reset the working directory to a specific commit
    ```

12. `git diff` - Shows differences between your working directory and commit.

    ```bash
    git diff                # Show changes not staged for commit
    git diff --staged       # Show changes staged for commit
    ```

13. `clone` - navigate to repository and clone by copying the url from git `code`, nb! use SSH

    ```bash
    cd ~/projects  # Example: Go to your projects folder
    git clone git@github.com:username/repository.git  # Using SSH
    ```

14. `.gitignore` - A file with lists of files that git should ignore when pushing
    
    **Common flags**:
    * Ignoring Specific Files:
  
        ```bash
        secret_config.json  # Ignore a specific file
        database.db         # Ignore a database file
        ```

    * Ignoring All Files of a Certain Type:

        ```bash
        *.log     # Ignore all log files
        *.tmp     # Ignore all temporary files
        *.bak     # Ignore all backup files
        ```
    
    * Ignoring Entire Directories:

        ```bash
       /node_modules/      # Ignore the node_modules folder (common in Node.js projects)
        __pycache__/        # Ignore Python cache files
        build/              # Ignore compiled files in a build directory
        dist/               # Ignore distribution files
        ```

    * Ignoring System Files (OS-Specific):

        ```bash
        .DS_Store  # macOS Finder metadata
        Thumbs.db  # Windows thumbnail database
        ```

    *   Ignoring IDE & Editor-Specific Files:

        ```bash
        .vscode/        # VS Code settings
        .idea/          # JetBrains IDEs (e.g., PyCharm, WebStorm)
        *.swp           # Vim swap files
        ```

    ### `Pull`, `fetch` and `merge`
    | Command    | Description                                                         | When to Use?                                              | Fetch Changes | Merge Automatically |
    |------------|---------------------------------------------------------------------|-----------------------------------------------------------|---------------|---------------------|
    | `git fetch` | Downloads new commits from the remote but does not update your working directory. | When you want to check for updates without merging yet.    | ✅ Yes         | ❌ No               |
    | `git pull`  | Downloads new commits and automatically merges them into your current branch. | When you want to update your local branch with the latest changes. | ✅ Yes         | ✅ Yes              |
    | `git merge` | Combines changes from one branch into another. Usually follows `git fetch`. | When you want to manually merge changes from another branch. | ❌ No          | ✅ Yes (if needed)   |

## 🔑 How to make SSH key

### 🛠️ This is a tutorial for how to make a `SSH key`

1. Check if you already have a `SSH key`

   ```bash
   ls -al ~/.ssh
   ```

   If output shows `id_ed25519` or `id_25519.pub`, you already have a key, if not precede.

2. Make a ed25519 SSH key by running the following command

   ```bash
   ssh-keygen -t ed25519 -C "your_email@example.com"
   ```

   **Explanation**:

   - `ssh-keygen` → Generates the key.
   - `-t ed25519` → Uses the Ed25519 algorithm (more secure than RSA).
   - `-C "your_email@example.com"` → Adds a comment (your email) for identification.

   **When prompted**:

   - Enter a file name → Press Enter to save it in the default location (`~/.ssh/id_ed25519`).
   - Enter a passphrase (optional) → For extra security, enter a passphrase (or press Enter for no passphrase).

3. Start SSH agent:

   ```bash
   eval "$(ssh-agent -s)"
   ```

   Than add your SSH key:

   ```bash
   ssh-add ~/.ssh/id_ed25519
   ```

   - If using Windows, run

     ```bash
     eval $(ssh-agent -s)
     ```

     then

     ```bash
     ssh-add ~/.ssh/id_ed25519
     ```

4. Copy the SSH key to add to GitHub

   ```bash
   cat ~/.ssh/id_ed25519.pub
   ```

5. Add the SSH key to GitHub

   1. Go to **GitHub** → SSH Keys Settings.
   2. Click **New SSH Key**.
   3. **Title**: Enter a descriptive name (e.g., "Laptop SSH Key").
   4. **Key Type**: Keep it as **Authentication Key**.
   5. **Key**: Paste your copied SSH key (`id_ed25519.pub`)
   6. Click **Add SSH Key**.

6. Test your SSH key

   ```bash
   ssh -T git@github.com
   ```

   if successful, you should see

   ```bash
   Hi username! You've successfully authenticated, but GitHub does not provide shell access.
   ```

7. To use SSH for a specific GitHub repository:

   Navigate to your local repository.
   Change the remote URL to SSH:

   ```bash
   git remote set-url origin git@github.com:username/repository.git
   ```

   Verify it:

   ```bash
   git remote -v
   ```

   It should display something like:

   ```bash
   origin  git@github.com:username/repository.git (fetch)
   origin  git@github.com:username/repository.git (push)
   ```

### 🎯 Summary of Commands

```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
cat ~/.ssh/id_ed25519.pub # Copy this key
ssh -T git@github.com
git remote set-url origin git@github.com:username/repository.git
```

### How to Add SSH Key to a Custom Location

1. If not existing, make the folders where you want to store the key. It is preferrable that this folder contains a `.ssh`, which should be a part of the `custom_location`.
   
    ```bash
    ssh-keygen -t ed25519 -f ~/path/to/custom_location/id_ed25519
    ```

    **Example**

    ```bash
    ssh-keygen -t ed25519 -f ~/Desktop/gruppe23_tihi/.ssh/id_ed25519
    ```

2. Add the SSH Key to the SSH Agent
   
   ```bash
    eval "$(ssh-agent -s)"
    ssh-add ~/path/to/custom_location/id_ed25519
    ```

3. In Github.com, add the key

    ```bash
    cat ~/path/to/custom_location/id_ed25519.pub
    ```
   
   * Add the SSH key to GitHub

   1. Go to **GitHub** → SSH Keys Settings.
   2. Click **New SSH Key**.
   3. **Title**: Enter a descriptive name (e.g., "Laptop SSH Key").
   4. **Key Type**: Keep it as **Authentication Key**.
   5. **Key**: Paste your copied SSH key (`id_ed25519.pub`)
   6. Click **Add SSH Key**.
4. Test the connection
   
    ```bash
    ssh -T git@github.com
    ```

    Which should output

    ```bash
    Hi <your-username>! You've successfully authenticated, but GitHub does not provide shell access.
    ```


## 📝 How to Create and Use a `.gitignore` File
The `.gitignore` file tells Git which files or directories to ignore so they won’t be tracked in the repository. This helps keep your repository clean by avoiding unnecessary files like compiled binaries, logs, or temporary files.

1. Navigte to **root folder** and create a `.gitignore` file

   ```bash
   touch .gitignore
    ```

2. Open it  in a text editor and make changes like

   ```text
    # Ignore compiled object files  
    *.o  
    *.out  
    *.exe  

    # Ignore all files in the `build/` directory  
    build/  

    # Ignore VS Code settings  
    .vscode/  

    # Ignore log files  
    *.log  

    # Ignore temporary files  
    *.tmp  
    .DS_Store  # macOS system file
    ```