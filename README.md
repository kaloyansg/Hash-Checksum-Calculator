# Checksum Calculator & File Integrity Verifier

## Overview
This project is a C++ application designed to traverse directory structures, calculate checksums for files using various algorithms (MD5, CRC32, Adler32), and verify file integrity by comparing current states against previously saved reports. It employs robust Object-Oriented Design patterns to handle file system operations, reporting, and concurrency control.

## Features
* **Multi-Algorithm Support**: Calculates checksums using MD5, CRC32, and Adler32.

* **File System Traversal**: Recursively processes directories and files.

* **Integrity Verification**: Compares current file states with saved records to detect modifications, deletions, or new files.

* **Progress Tracking**: Real-time progress reporting using the Observer pattern.

* **Flow Control**: Mechanism to pause and resume the calculation process.

* **Serialization**: Saves and loads checksum reports (supports XML format).


## Architecture & Design Patterns

The solution implements several key design patterns to ensure modularity and extensibility:

### Composite Pattern:

* Used to represent the file system.

* `SystemItem` (Abstract Base), `Directory` (Composite), and `File` (Leaf).

### Visitor Pattern:

* Used to perform operations on the file system tree without modifying the classes.

* `Visitor` (Interface), `PrintVisitor` (for displaying structure), `ProcessingVisitor` (for calculating hashes).

### Builder Pattern:

* Used to construct the file system representation.

* `FileSystemBuilder`, `SimpleFileSystemBuilder`, `SmartFileSystemBuilder`.

### Strategy Pattern:

* Encapsulates different checksum algorithms.

* `ChecksumCalculator` (Interface), `MD5_CC`, `CRC32_CC`, `Alder32_CC`.

### Observer Pattern:

* Used for the event handling system (e.g., updating the UI/Console on progress).

* `Observable`, `Observer`, `ProgressReporter`.


## Project Structure
`src/` & `headers/`
### Core Logic:
* `main.cpp`: Entry point of the application.
* `SystemItem`, `File`, `Directory`: File system representation.

### Calculators:
* `MD5_CC`, `CRC32_CC`, `Alder32_CC`: Hashing algorithm implementations.

### File I/O & Reports:

* `XML_RW`: Handles reading/writing XML reports.

* `ReportWriter`, `Saver`: Manages outputting results.

* `HashStreamWriter`: Helper for stream operations.

### Utilities:

* `Comperator`: Logic to compare two datasets/file states.

* `PauseController`: Manages thread/process suspension.

`test/`
Contains unit tests ensuring reliability for individual components:

* `testResources`: Sample files (music, photos, text) used for verification.

* `*Test.cpp`: Individual test suites for components like `MD5_CC`, `Saver`, `XML_RW`, etc.
