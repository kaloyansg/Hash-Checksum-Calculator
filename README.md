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

---

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

---

## Usage Examples
### Calculate Command
```
>> "Checksum\Checksum\x64\Debug\Checksum.exe" --calculate="testDir" -m

--- Mode: Checksum Calculation ---
Controls: [p]ause, [r]esume, [s]ave state, [l]oad state, [q]uit
[testDir]
Processing abc.txt...
        [Done] abc.txt -> 900150983CD24FB0D6963F7D28E17F72
  - abc.txt
Processing cs.txt...
        [Done] cs.txt -> C4029AD86623E3E81B62F0B4BE2DF24F
  - cs.txt
Processing Hello world.txt...
        [Done] Hello world.txt -> 1D94DD7DFD050410185A535B9575E184
  - Hello world.txt
  [music]
        [kanye]
Processing 10.Hell Of A Life.m4a...
        [Done] 10.Hell Of A Life.m4a -> C2FC09E4071ED727D70C691C03750B86
      - 10.Hell Of A Life.m4a
    [mix]
        [vrgo]
Processing 1.TOCHNA DOZA.m4a...
        [Done] 1.TOCHNA DOZA.m4a -> 4FE9FB0BFF9D8714A9B8986544A00D21
      - 1.TOCHNA DOZA.m4a
Processing 4.HOW - Unreleased Bonus.m4a...
        [Done] 4.HOW - Unreleased Bonus.m4a -> 5CC1171D9A2165B16D83F4B0D1200C25
      - 4.HOW - Unreleased Bonus.m4a
  [photos]
Processing image00002.jpeg...
        [Done] image00002.jpeg -> 7C33AF3DFC910959B3DDC4F1CD99AA3F
    - image00002.jpeg
  [wordFIles]
Processing New Microsoft Word Document.docx...
        [Done] New Microsoft Word Document.docx -> 57C57FF0AABE41064783BC326BF8ED6E
    - New Microsoft Word Document.docx

--- Scan Complete ---
Enter [q] to exit...
q

>>
```


### Checksums Command
```
>> "Checksum\Checksum\x64\Debug\Checksum.exe" --checksums="testDir" -m "savedChecksumTest1.txt"
testDir\abc.txt: OK
testDir\cs.txt: NEW
testDir\Hello world.txt: MODIFIED
testDir\music\kanye\10.Hell Of A Life.m4a: OK
testDir\music\vrgo\1.TOCHNA DOZA.m4a: OK
testDir\music\vrgo\4.HOW - Unreleased Bonus.m4a: OK
testDir\photos\image00002.jpeg: OK
testDir\wordFIles\New Microsoft Word Document.docx: OK
testDir\test2.txt: REMOVED

>>
```

### Report Command
```
>> "Checksum\Checksum\x64\Debug\Checksum.exe" --report="testDir" -m

--- Mode: Structure Report ---
900150983CD24FB0D6963F7D28E17F72 *testDir\abc.txt
C4029AD86623E3E81B62F0B4BE2DF24F *testDir\cs.txt
1D94DD7DFD050410185A535B9575E184 *testDir\Hello world.txt
C2FC09E4071ED727D70C691C03750B86 *testDir\music\kanye\10.Hell Of A Life.m4a
4FE9FB0BFF9D8714A9B8986544A00D21 *testDir\music\vrgo\1.TOCHNA DOZA.m4a
5CC1171D9A2165B16D83F4B0D1200C25 *testDir\music\vrgo\4.HOW - Unreleased Bonus.m4a
7C33AF3DFC910959B3DDC4F1CD99AA3F *testDir\photos\image00002.jpeg
57C57FF0AABE41064783BC326BF8ED6E *testDir\wordFIles\New Microsoft Word Document.docx

>>
```

### Report Command (XML)
```
>> "Checksum\Checksum\x64\Debug\Checksum.exe" --report="testDir" -m -x

--- Mode: Structure Report ---
<checksum>
  <item>
    <mode>binary</mode>
    <checksum>900150983CD24FB0D6963F7D28E17F72</checksum>
    <path>testDir\abc.txt</path>
    <size>3</size>
  </item>
  <item>
    <mode>binary</mode>
    <checksum>C4029AD86623E3E81B62F0B4BE2DF24F</checksum>
    <path>testDir\cs.txt</path>
    <size>168</size>
  </item>
  <item>
    <mode>binary</mode>
    <checksum>1D94DD7DFD050410185A535B9575E184</checksum>
    <path>testDir\Hello world.txt</path>
    <size>13</size>
  </item>
  <checksum>
    <checksum>
      <item>
        <mode>binary</mode>
        <checksum>C2FC09E4071ED727D70C691C03750B86</checksum>
        <path>testDir\music\kanye\10.Hell Of A Life.m4a</path>
        <size>10627286</size>
      </item>
    </checksum>
    <checksum>
    </checksum>
    <checksum>
      <item>
        <mode>binary</mode>
        <checksum>4FE9FB0BFF9D8714A9B8986544A00D21</checksum>
        <path>testDir\music\vrgo\1.TOCHNA DOZA.m4a</path>
        <size>7632798</size>
      </item>
      <item>
        <mode>binary</mode>
        <checksum>5CC1171D9A2165B16D83F4B0D1200C25</checksum>
        <path>testDir\music\vrgo\4.HOW - Unreleased Bonus.m4a</path>
        <size>5127229</size>
      </item>
    </checksum>
  </checksum>
  <checksum>
    <item>
      <mode>binary</mode>
      <checksum>7C33AF3DFC910959B3DDC4F1CD99AA3F</checksum>
      <path>testDir\photos\image00002.jpeg</path>
      <size>337837</size>
    </item>
  </checksum>
  <checksum>
    <item>
      <mode>binary</mode>
      <checksum>57C57FF0AABE41064783BC326BF8ED6E</checksum>
      <path>testDir\wordFIles\New Microsoft Word Document.docx</path>
      <size>443457</size>
    </item>
  </checksum>
</checksum>

>>
```
