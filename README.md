# Variable Storage
## Description
The Variable Storage plugin provides a structure and a set of functions that allow you to write multiple data into a single structure and then retrieve any written data from it.

## Requirement

Target version : UE5.0 ～ 5.5

Target platform : Windows

## Warning
  - Variable Storage can only contain these data types: **Bool, Byte, Int32, Int64, Float, Name, String, Text, Vector, Rotator, Transform, Linear Color.**
  - **Data names cannot be repeated in Variable Storage.** When merging Variable Storage structures, data with the same name will replace each other.

## Content
* Variable Storage Structure
  - Contains data with unique names.

![Снимок 6](https://github.com/user-attachments/assets/c618f2b2-d27d-4ee2-bc70-9f47c4b46150)

* Set Data To Variable Storage 
  - Writes data to the variable store under the name specified in "Data name".

![Снимок 1](https://github.com/user-attachments/assets/09176942-532c-4059-9ea6-b80940fcd74d)

* Set Data To Variable Storage (Array)
  - Writes an array of data to the variable store under the name specified in "Data Name".

![Снимок 2](https://github.com/user-attachments/assets/ee84a2f2-2b4a-4a44-9679-327af34b8a6e)

* Get Data To Variable Storage
  - Gets data from the variable store with the name specified in "Data name".

![Снимок 3](https://github.com/user-attachments/assets/0c579df6-e967-4a05-8e7b-27151ec51123)

* Get Data To Variable Storage (Array)
  - Gets Array data from the variable store with the name specified in "Data name".
  
![Снимок 4](https://github.com/user-attachments/assets/4fef224e-1ed1-4880-9f8f-80e6f93a0f6b)

* Multi Append Variable Storage
  - Combines an array of Variable Storage structures into a single Variable Storage structure.

![Снимок 5](https://github.com/user-attachments/assets/f7c8feb2-c89d-4bcc-b377-4a289fe63bdb)
