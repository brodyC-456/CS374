File Scanner

Usage: filescanner [directory]

This is a simple file scanner that opens a specified directory, reads its contents, and prints the total size (in bytes) of all of the contents. It uses the current directory by default. 
If there are subdirectories, it recursively reads them and adds their total byte size to the total, printing it separately and before its parent. 
If a directory cannot be read for any reason, then an error message is displayed and the subdirectory is not added to the total.
