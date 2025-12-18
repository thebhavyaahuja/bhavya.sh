[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Qiz9msrr)
# bhavya.sh (CShell)

## Functionalities:

### **Display**

#### `getHomeDirectory()`

- **Purpose**: Fetches and stores the current working directory in `homeDir`.
- **Functionality**:
  - Uses `getcwd()` to get the current directory path.
  - If `getcwd()` fails, it prints an error message using `perror()`.

#### `display()`

- **Purpose**: Displays a custom shell prompt with details like username, system name, current directory, and optional process information.
- **Functionality**:
  - **System Name**:
    - Retrieves system information using `uname()`.
  - **Username**:
    - Fetches the username using `getpwuid()` based on the current user’s ID (`getuid()`).
    - Prints "Username: Unknown" if `getpwuid()` fails.
  - **Current Directory**:
    - Compares `path` (current directory) with `homeDir` (home directory) to decide how to display the directory in the prompt. (uses absolute path if path is before the home directory, else ~/[relative path]>)
  - **Prompt Formatting**:
    - Uses color codes:
      - `BRIGHT_CYAN` for username and system name.
      - `BRIGHT_YELLOW` for directory path.
      - `RESET` to reset colors.
      - If `timeFlag` is set:
      - Displays time taken for foreground process as well.

### **Hop**

#### `hop` Function

- **Purpose**
The `hop` function changes the current working directory based on the provided `path` argument. It handles several special cases and provides feedback if the directory change fails.

#### Functionality

1. **Get Current Directory**:
   - Uses `getcwd()` to store the current directory in `currDir`.

2. **Change Directory**:
   - **`"~"`**: Changes to the home directory stored in `homeDir`.
   - **`"-"`**: Changes to the previous directory stored in `prevDir`.
   - **`.`**: Remains in the current directory (no action taken).
   - **`".."`**: Moves up one directory level.
   - **Other Paths**: Attempts to change to the specified directory. If the path is invalid, prints an error message in red and stops further execution.

3. **Update Previous Directory**:
   - After a successful directory change, updates `prevDir` with the old current directory (`currDir`).

4. **Display Current Directory**:
   - Prints the new current directory in bright yellow.

#### Error Handling
- If `chdir()` fails (i.e., the directory does not exist), it prints an error message using red color and halts execution.

### **Tokenizing Input**

#### `tokenise_space` Function

- **Purpose**: Splits a string by spaces and processes commands separated by `&`.
- **Logic**:
  1. **Copy and Tokenize**: Creates a copy of `str` and splits it by spaces.
  2. **Process Tokens**: Copies tokens to `list`, handling `&` as a delimiter and calling `handle_piping()`.
  3. **Reset**: Prepares for the next set of tokens.

#### `semicolonTokenise` Function

- **Purpose**: Splits the `command` string by `;` to process individual commands.
- **Logic**:
  1. **Copy and Tokenize**: Copies `command` and splits by `;`.
  2. **Trim and Process**: Removes leading spaces and calls `tokenise_space()` for further processing.

### **Reveal**

#### `reveal` Function

#### Purpose
Lists and displays details of files and directories based on provided flags.

#### Functionality

1. **Parse Flags**:
   - **`a`**: Show hidden files.
   - **`l`**: Show detailed information.
   - if it contains both `a` and `l` in some format, both the flags are set to 1.

2. **Resolve Path**:
   - **Default**: Uses current directory `"."`.
   - **`~`**: Replaces with home directory.
   - **`-`**: Replaces with previous directory.

3. **Check Path Type**:
   - **Directory**:
     - Opens and reads directory contents.
     - Filters out hidden files if `-a` is not set.
     - Stores entries, sorts them, and prints them with optional details.
   - **Regular File**:
     - Prints details for the file.
   - **Unsupported Type**:
     - Prints an error message.

#### Helper Functions

- **`compare`**: Used by `qsort` to sort directory entries alphabetically.
- **`print_details`**: Displays file/directory name with color coding and, if requested, additional details (e.g., permissions, owner, size, modification time).

### **Log**

#### `addLog()`
- **Purpose**: Adds the current command to the log file, ensuring no duplicates and maintaining a maximum of 15 entries.
- **Logic**:
  1. **Read Existing Log**: Opens the log file, reads existing commands, and stores them in an array.
  2. **Check for Duplicates**: Avoids adding the same command consecutively.
  3. **Update Log**: Adds the new command to the log, either appending it or replacing the oldest entry if the log is full.
  4. **Write to Log**: Writes the updated list of commands back to the log file.

#### `displayLog()`
- **Purpose**: Displays all entries from the log file.
- **Logic**:
  1. **Open Log**: Opens the log file for reading.
  2. **Print Entries**: Reads and prints each line from the log file to the console.

#### `purgeLog()`
- **Purpose**: Clears the log file.
- **Logic**:
  1. **Open and Close Log**: Opens the log file in write mode (which truncates it) and then closes it.
  2. **Notify Success**: Prints a confirmation message indicating successful log purging.

#### `executeLog(int n)`
- **Purpose**: Executes a command from the log file based on the line number `n`.
- **Logic**:
  1. **Open Log**: Opens the log file for reading.
  2. **Read and Execute**: Finds the command at the specified line number, sets it as the current command, and processes it using `semicolonTokenise()`.

### **System Commands**

#### Function: `check_background_processes`
- **Purpose**: Monitors and updates the status of background processes.
- **Logic**:
  1. **Check Status**: Uses `waitpid` with flags to handle background process states.
  2. **Update Status**: Prints and updates the status of each background process (e.g., "Exited", "Terminated", "Stopped", "Running").

#### Function: `isCustom`
- **Purpose**: Identifies and executes custom commands.
- **Logic**:
  1. **Command Check**: Compares the first token of `list` to known custom commands (e.g., `hop`, `log`, `reveal`, `proclore`, `seek`, `activities`, `ping`, `bg`, `fg`, `iMan`, `neonate`, `mk_hop`).
  2. **Execute Command**: Calls appropriate functions based on the command and its parameters.

#### Function: `isValid`
- **Purpose**: Validates and executes standard and custom commands.
- **Logic**:
  1. **Handle Redirection**: Checks for and processes I/O redirection operators (`>`, `>>`, `<`, `|`).
  2. **Custom Commands**: Calls `isCustom()` to handle custom commands.
  3. **Command Validation**: Checks if the command is valid. If not, prints an error message and exits.
  4. **Background/Foreground Handling**: Sets the process as background or foreground based on the presence of `&`.
  5. **Fork and Execute**: Forks a new process to execute the command using `execvp`. Updates process tracking and handles status based on whether the process is foreground or background.

### **Proclore**

#### Function: `process_Status`

- **Purpose**: Retrieves and displays the state of a process.
- **Parameters**: 
  - `pid`: Process ID of the target process.
- **Logic**:
  1. **Path Creation**: Constructs the path to the `/proc/<pid>/status` file.
  2. **File Opening**: Opens the status file for reading.
  3. **State Extraction**: Reads through the file to find and print the line starting with "State:".
  4. **File Closing**: Closes the file.

#### Function: `virtualMemory`

- **Purpose**: Retrieves and displays the virtual memory size of a process.
- **Parameters**: 
  - `pid`: Process ID of the target process.
- **Logic**:
  1. **Path Creation**: Constructs the path to the `/proc/<pid>/status` file.
  2. **File Opening**: Opens the status file for reading.
  3. **Memory Size Extraction**: Reads through the file to find and print the line starting with "VmSize:".
  4. **File Closing**: Closes the file.

#### Function: `getExecutablePath`

- **Purpose**: Retrieves and displays the executable path of a process.
- **Parameters**: 
  - `pid`: Process ID of the target process.
- **Logic**:
  1. **Path Creation**: Constructs the path to the `/proc/<pid>/exe` symbolic link.
  2. **Link Reading**: Reads the symbolic link to get the executable path.
  3. **Path Formatting**: Formats the path to replace home directory with `~` if applicable.
  4. **Path Display**: Prints the executable path.

#### Function: `noPidGiven`

- **Purpose**: Displays information about the current process when no specific PID is provided.
- **Logic**:
  1. **Get Current PID**: Retrieves the current process ID and its process group ID.
  2. **Display Information**: Calls `process_Status`, `virtualMemory`, and `getExecutablePath` to display information about the current process.

### **Seek**

#### Function: `print_error`

- **Purpose**: Prints an error message in red.
- **Parameters**:
  - `msg`: Error message to be printed.
- **Logic**:
  - Uses `fprintf` to print the error message to `stderr` with red color formatting.

#### Function: `search_directory`

- **Purpose**: Recursively searches for files or directories matching a target name.
- **Parameters**:
  - `base`: Base path for the search.
  - `dir_path`: Directory path to search in.
  - `target`: Target name to search for.
  - `is_dir`: Flag indicating if directories should be searched.
  - `is_file`: Flag indicating if files should be searched.
  - `count`: Pointer to an integer that counts the number of matches.
  - `last`: Pointer to a string that stores the path of the last match.
- **Logic**:
  1. **Path Construction**: Constructs the path to open the directory.
  2. **Directory Reading**: Opens and reads the directory.
  3. **File/Directory Matching**:
     - Checks if the entry is a directory or a file and matches the target name.
     - Prints the entry with color formatting based on type (blue for directories, green for files).
  4. **Recursive Search**: Recursively searches subdirectories.
  5. **Directory Closing**: Closes the directory after processing.

#### Function: `seek_command`

- **Purpose**: Parses command-line arguments to search for files or directories and optionally execute or display the content of the found items.
- **Logic**:
  1. **Flag Parsing**: Parses command-line arguments to set flags for directory (`-d`), file (`-f`), and execute (`-e`).
  2. **Target and Base Path**:
     - Sets the target name and base path for the search.
     - Displays an error if no target is specified.
  3. **Search Directory Tree**: Calls `search_directory` to search for matching files or directories.
  4. **Match Handling**:
     - Prints an error message if no matches are found.
     - If `-e` flag is set and exactly one match is found, executes or displays the file or directory:
       - Changes directory if the match is a directory.
       - Reads and displays the content if the match is a file.

### **myshrc**

#### Function: `mk_hop`

- **Purpose**: Creates a directory with the specified name and changes into it.
- **Parameters**: None directly, uses `list[1]` for the directory name.
- **Logic**:
  1. **Create Directory**:
     - Uses `mkdir` to create a new directory with permissions `0755`.
     - Prints an error message if `mkdir` fails.
  2. **Change Directory**:
     - Changes the current working directory to the newly created directory using `chdir`.

#### Function: `openMySH`

- **Purpose**: Opens the `.myshrc` file, processes it to handle aliases and specific flags.
- **Parameters**: None directly.
- **Logic**:
  1. **Initialize Flags**:
     - Sets `mk_hop_flag` and `hop_seek_flag` to `0`.
     - Constructs the path to the `.myshrc` file in the home directory.
  2. **Open `.myshrc` File**:
     - Opens the file for reading.
     - Prints an error message if the file cannot be opened.
  3. **Process File Lines**:
     - Reads the file line by line.
     - **Alias Handling**:
       - Checks if a line starts with "alias".
       - Parses the alias name and the associated command.
       - Replaces the command in `list` if the alias matches the first command in `list`.
       - Adjusts `list` to include the expanded alias command.
     - **`mk_hop` Flag Handling**:
       - Sets `mk_hop_flag` to `1` if a line starts with "mk_hop".
  4. **Close File**:
     - Closes the `.myshrc` file after processing.

### **Redirection**

#### Function: `copylistfunction`

- **Purpose**: Creates a backup of the current command list.
- **Parameters**: None.
- **Logic**:
  1. **Backup**:
     - Copies the contents of `list` to `copyList` to preserve the original command list before handling input/output redirection.

#### Function: `handle_io_redirection`

- **Purpose**: Handles input and output redirection based on the provided command list.
- **Parameters**: None directly.
- **Logic**:
  1. **Backup Original List**:
     - Calls `copylistfunction()` to back up the original command list.
  2. **Initialize File Descriptors**:
     - Initializes `fd_in` and `fd_out` for input and output redirection.
     - Saves original `stdin` and `stdout` file descriptors (`saved_stdin` and `saved_stdout`).
  3. **Process Redirection**:
     - Iterates over the `copyList` to handle redirection operators (`>`, `>>`, `<`):
       - **Output Redirection (`>` and `>>`)**:
         - Redirects `stdout` to the specified file.
         - Uses `O_WRONLY | O_CREAT | O_TRUNC` for overwrite or `O_APPEND` for append.
       - **Input Redirection (`<`)**:
         - Redirects `stdin` from the specified file.
       - **Update `list[]`**:
         - Copies valid command arguments to `list[]` excluding redirection operators and file names.
  4. **Validate and Execute Command**:
     - Validates and executes the command if there are remaining arguments.
     - Calls `isValid()` to check if the command is valid.
  5. **Restore Original I/O**:
     - Restores original `stdin` and `stdout` if they were redirected.
     - Closes the duplicated file descriptors.
  6. **Flush Output**:
     - Ensures all output is flushed.

### **Piping**

#### Function: `sameListfunction`

- **Purpose**: Creates a backup of the current command list to preserve it before handling piping.
- **Parameters**: None.
- **Logic**:
  1. **Backup**:
     - Copies the contents of `list` to `sameList` to preserve the original command list before processing piping.

#### Function: `handle_piping`

- **Purpose**: Handles piping between commands, allowing the output of one command to be used as the input to the next.
- **Parameters**: None directly.
- **Logic**:
  1. **Backup Command List**:
     - Calls `sameListfunction()` to back up the original command list.
  2. **Initialize File Descriptors**:
     - Saves the original `stdin` and `stdout` file descriptors (`std_in` and `std_out`).
     - Initializes pipe file descriptors (`pipefd`) and `prev_fd` for chaining pipes.
  3. **Process Commands**:
     - Iterates over the command list:
       - **Command Handling**:
         - If the current element is not a pipe (`|`), copies it to `list[]`.
         - If a pipe is encountered:
           - **Pipe Creation**:
             - Creates a new pipe using `pipe(pipefd)`.
           - **Forking**:
             - Forks a child process:
               - **Child Process**:
                 - Redirects `stdin` from the previous pipe (if any).
                 - Redirects `stdout` to the write end of the current pipe.
                 - Closes unused pipe ends and executes redirection and the command using `handle_io_redirection()`.
               - **Parent Process**:
                 - Waits for the child process to finish.
                 - Closes the write end of the pipe.
                 - Saves the read end of the pipe to be used as `stdin` for the next command.
                 - Resets the command list for the next segment.
  4. **Final Command**:
     - If there's a remaining pipe (`prev_fd`), sets it as `stdin` and closes it.
     - Executes the final command with `handle_io_redirection()`.
  5. **Restore Original I/O**:
     - Restores the original `stdin` and `stdout` file descriptors.

### **isValid**

#### Function: `check_background_processes`

- **Purpose**: Monitors and updates the status of background processes.
- **Parameters**: None.
- **Logic**:
  1. **Wait for Processes**:
     - Uses `waitpid` with flags `WNOHANG`, `WUNTRACED`, and `WCONTINUED` to check for background processes' status.
  2. **Update Status**:
     - Updates the status of background processes in the `activities` array based on the process' exit, signal termination, stoppage, or continuation.

#### Function: `isCustom`

- **Purpose**: Identifies and executes custom commands defined by the user.
- **Parameters**: None.
- **Logic**:
  1. **Command Identification**:
     - Checks the first element of `list[]` to determine the command type (`hop`, `log`, `reveal`, `proclore`, `seek`, `activities`, `ping`, `bg`, `fg`, `iMan`, `neonate`, `mk_hop`).
  2. **Execute Custom Commands**:
     - Executes the corresponding function based on the command type.
     - Handles specific cases for each command, such as purging or executing logs, revealing directories, handling process status, etc.

#### Function: `isValid`

- **Purpose**: Validates and executes commands, handling redirections and background/foreground processes.
- **Parameters**: None.
- **Logic**:
  1. **Open Configuration**:
     - Calls `openMySH()` to read and apply shell configuration.
  2. **Handle Redirections**:
     - Processes redirection symbols (`>`, `>>`, `<`, `|`) using `handle_io_redirection()`.
  3. **Custom Command Check**:
     - Calls `isCustom()` to handle custom commands.
  4. **Command Validation**:
     - Checks if the command is valid.
  5. **Process Management**:
     - Handles background and foreground processes.
     - Uses `fork()` to create a child process and `execvp()` to execute the command.
     - Manages process tracking and status updates.
     - Measures execution time and handles errors based on process exit status.

### **is_command_valid**

#### Description

The `is_command_valid` function checks if a given command is valid. It determines validity by verifying if the command exists and is executable either as a full path or through the directories listed in the `PATH` environment variable.

### **Signals**

#### Function: `sendSignals`

- **Purpose**: Sends a specified signal to a process with the given PID.
- **Parameters**:
  - **`pid`**: Process ID to which the signal is sent.
  - **`signal`**: Signal number to send.
- **Logic**:
  1. **Send Signal**:
     - Uses `kill` to send the specified signal to the process.
     - Prints a success message if the signal is sent successfully.
     - Calls `check_background_processes` to update the status of background processes.
  2. **Error Handling**:
     - Prints an error message if the signal could not be sent.

#### Function: `handle_sigint`

- **Purpose**: Handles the SIGINT signal (Ctrl+C) to terminate foreground processes.
- **Parameters**:
  - **`sig`**: Signal number (SIGINT).
- **Logic**:
  1. **Terminate Foreground Processes**:
     - Iterates through the `activities` array.
     - Sends SIGINT (signal number 2) to all foreground processes that are not already stopped.
     - Calls `sendSignals` to handle the termination.

#### Function: `kill_all_processes`

- **Purpose**: Terminates all running processes by sending SIGKILL (signal number 9) for Ctrl + D input.
- **Parameters**: None.
- **Logic**:
  1. **Terminate Running Processes**:
     - Iterates through the `activities` array.
     - Sends SIGKILL to processes that are currently running.
     - Calls `sendSignals` to handle the termination.

#### Function: `handle_sigstp`

- **Purpose**: Handles the SIGTSTP signal (Ctrl+Z) to stop the foreground process.
- **Parameters**:
  - **`sig`**: Signal number (SIGTSTP).
- **Logic**:
  1. **Stop Foreground Process**:
     - Uses `killpg` to send SIGTSTP to the process group of the current foreground process.
     - Updates the status of the stopped process in the `activities` array.
     - Sets the foreground process ID to -1.
  2. **Restore Shell Control**:
     - Sets the shell back as the foreground process group leader using `tcsetpgrp`.
     - Prints a message indicating that the shell is ready for new commands.

### **fg and bg**

#### Function: `continue_bg_process`

- **Purpose**: Resumes a background process that is currently stopped.
- **Parameters**:
  - **`pid`**: Process ID of the process to be resumed.
- **Logic**:
  1. **Find the Process**:
     - Iterates through the `activities` array to locate a stopped background process with the specified PID.
     - Checks that the process status is "Stopped" and is currently a background process.
  2. **Resume the Process**:
     - Sends SIGCONT to the process to resume its execution.
     - Updates the process status to "Running" and prints a confirmation message.
     - Handles errors if the signal could not be sent.
  3. **Process Not Found**:
     - Prints an error message if no process with the specified PID is found.

#### Function: `bring_to_foreground`

- **Purpose**: Brings a background process to the foreground and waits for its status.
- **Parameters**:
  - **`pid`**: Process ID of the process to be brought to the foreground.
- **Logic**:
  1. **Find and Update the Process**:
     - Iterates through the `activities` array to find a process with the specified PID that is either "Running" or "Stopped" and is a background process.
     - Sets the process to foreground and updates its status to "Running".
  2. **Resume and Wait for the Process**:
     - Sends SIGCONT to the process to resume it if it was stopped.
     - Uses `waitpid` to wait for the process to either complete or stop again.
     - Updates the process status based on whether it was stopped, exited, or terminated by a signal.
  3. **Restore Terminal Control**:
     - Sets the shell as the foreground process group leader using `tcsetpgrp`.

### **Neonate**

#### Function: `fetch_latest_pid`

- **Purpose**: Retrieves the PID of the most recently created process.
- **Parameters**: None.
- **Logic**:
  1. **Open `/proc` Directory**:
     - Opens the `/proc` directory to list process entries.
  2. **Read Directory Entries**:
     - Iterates over directory entries and checks if the entry names are numeric (indicating PIDs).
  3. **Fetch File Metadata**:
     - For each numeric entry, fetches file metadata to get the creation time.
  4. **Determine Latest PID**:
     - Compares creation times to find the most recent PID.
  5. **Close Directory**:
     - Closes the directory after processing all entries.
  6. **Return PID**:
     - Returns the PID of the most recent process or `-1` if an error occurs.

#### Function: `check_key_press`

- **Purpose**: Checks if a key has been pressed on the standard input.
- **Parameters**: None.
- **Logic**:
  1. **Disable Canonical Mode**:
     - Temporarily modifies terminal settings to disable canonical mode and echo.
  2. **Set Non-Blocking Input**:
     - Sets the standard input to non-blocking mode.
  3. **Check for Key Press**:
     - Reads a character from standard input to determine if a key press occurred.
  4. **Restore Terminal Settings**:
     - Restores the original terminal settings after checking.
  5. **Return Result**:
     - Returns `1` if a key was pressed, otherwise returns `0`.

#### Function: `neonate`

- **Purpose**: Periodically retrieves and prints the most recent PID and checks for key presses.
- **Parameters**:
  - **`interval_seconds`**: The time interval between checks for the latest PID.
- **Logic**:
  1. **Retrieve Latest PID**:
     - Calls `fetch_latest_pid()` to get the most recent PID and prints it.
  2. **Periodic Sleep and Key Press Check**:
     - Sleeps for `0.1` seconds in a loop for the specified interval.
     - Checks for key presses using `check_key_press()`.
     - Exits the loop if the 'x' key is pressed.

### **Networking (iMan)**

#### Function: `strip_html_tags`

- **Purpose**: Removes HTML tags from a given string, leaving only the text content.
- **Parameters**:
  - **`src`**: The source string potentially containing HTML tags.
  - **`dest`**: The destination string where the text content will be copied.
- **Logic**:
  1. **Detect HTML Tags**:
     - Uses a flag `inside_tag` to identify characters within HTML tags.
     - Skips characters between `<` and `>` (HTML tags).
  2. **Copy Non-Tag Characters**:
     - Copies characters that are outside of HTML tags to the destination string.
  3. **Null-Terminate**:
     - Null-terminates the destination string after processing.

#### Function: `print_webpage`

- **Purpose**: Fetches the content from a URL using `curl`, removes HTML tags, and prints the cleaned text.
- **Parameters**:
  - **`url`**: The URL to fetch the content from.
- **Logic**:
  1. **Create a Pipe**:
     - Creates a pipe to transfer data between processes.
  2. **Fork Process**:
     - **Child Process**:
       - Executes `curl` to fetch the webpage content and writes it to the pipe.
     - **Parent Process**:
       - Reads the content from the pipe.
       - Strips HTML tags from the content.
       - Prints the cleaned text.
  3. **Handle Errors**:
     - Prints errors if `pipe`, `fork`, or `execvp` fail.
  4. **Close Pipe Ends**:
     - Closes unused ends of the pipe in both parent and child processes.
  5. **Wait for Child Process**:
     - Waits for the child process to finish execution.

#### Function: `iMan`

- **Purpose**: Constructs a URL for fetching manual pages and invokes `print_webpage` to process and display the content.
- **Logic**:
  1. **Construct URL**:
     - Formats the URL to fetch manual pages for a given command (`manCommand`).
  2. **Print Webpage**:
     - Calls `print_webpage` with the constructed URL to fetch and display the manual page content.
