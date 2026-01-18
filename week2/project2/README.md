File Scanner

Usage: filescanner [directory]

This is a simple file scanner that opens a specified directory, reads its contents, and prints the total size (in bytes) of all of the contents. It uses the current directory by default. 
If there are subdirectories, it recursively reads them and adds their total byte size to the total, printing it separately and before its parent. 
If a directory cannot be read for any reason, then an error message is displayed and the subdirectory is not added to the total.

<img width="1300" height="1011" alt="function_diagram_project_2" src="https://github.com/user-attachments/assets/932fc91c-b851-4f73-9496-31f35adc0ef9" />
