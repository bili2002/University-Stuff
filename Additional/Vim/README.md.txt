# Vim Keystroke Counter and Checkpoint Management

This Vim configuration provides a keystroke counter and checkpoint management functionality. It allows you to keep track of the number of keystrokes and save/load copies of files as checkpoints.

## Keystroke Counter

The keystroke counter keeps track of the number of keystrokes in Vim. It counts keystrokes in normal mode, insert mode, command-line mode, and visual mode.

### Usage

- `GetCurrent`: Displays the current count of keystrokes.
- `SaveCheckpoint [filename]`: Saves a copy of the current file as a checkpoint with the provided filename.
- `LoadCheckpoint [filename]`: Loads a checkpoint file and replaces the content of the current file with the content of the loaded checkpoint.

### Note

- The checkpoint files are stored in a hidden directory, `~/.hidden_directory` by default. You can modify the target directory path by updating the `target_directory` variable in the code.
- Saving a checkpoint also saves the current count of keystrokes in a separate file.
- Loading a checkpoint restores the saved count of keystrokes.

## Installation

To use this configuration:

1. Copy plugin/ksc.vim to your vimfiles/plugin folder
2. Restart Vim or source the configuration file.
3. You can now use the keystroke counter and checkpoint management commands.
