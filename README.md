# Text-Editor

## Introduction
Welcome to my lightweight Text-Editor app! This application allows you to create, take, edit and insert text files. It has a barebones interface (this will be shown down the slides) which allows you to do the minimum essentials for note taking.   

![Image 1](https://user-images.githubusercontent.com/44689249/119993682-6c081700-bffe-11eb-9349-05977c9501b0.JPG)


## Usage Guide

Simple download the `texteditor_Bertrand.exe` file and double click to open. Select "Trust this application" and run the exe file. The text-editor screen will appear and you can begin using it!

> Unfortunately, there is no available version for Mac/Linux users at the moment.

## Available Features
### Files
![2](https://user-images.githubusercontent.com/44689249/119994250-010b1000-bfff-11eb-932d-013941b58f3f.jpg)
 1. `New File`: Starting on a fresh new file.
Under the `File` tab on the top-left corner, click on the `New File` button.
 2. `Open File`: Opening an existing text file to edit
Under the `File` tab on the top-left corner, click on the `Open File` button or press `Ctrl-O`on your keyboard.
 3. `Insert File`: Insert text from an existing text file into your current editor.
Under the `File` tab on the top-left corner, click on the `Insert File` button or press `Ctrl-I`on your keyboard.
 4. `Save File`: Save your existing changes on the text editor. If no save file has been created before, the editor will prompt you to create a new save file.
 Under the `File` tab on the top-left corner, click on the `Save File` button or press `Ctrl-S`on your keyboard.
 5. `Save File As`: Save your existing changes on the text editor as a file name of your choice.
Under the `File` tab on the top-left corner, click on the `Save File As...` button or press `Ctrl-Shift-S`on your keyboard.
 6. `New View`: Duplicate your existing text editor page onto a new text editor page.
Under the `File` tab on the top-left corner, click on the `New View` button or press `Alt-V`on your keyboard.
 7. `Close View`: Close your existing text editor page.
Under the `File` tab on the top-left corner, click on the `Close View` button or press `Ctrl-W`on your keyboard.
 9. `Exit`: Exit the text editor application
Under the `File` tab on the top-left corner, click on the `Exit` button or press `Ctrl-Q`on your keyboard.

### Edit
![3](https://user-images.githubusercontent.com/44689249/119995751-7e835000-c000-11eb-9d8a-c9865765405d.jpg)
 1. `Undo`: Undo your last added word.
Under the `Edit` tab on the top-left corner, click on the `Undo` button or press `Ctrl-Z` on your keyboard.
> The undo feature only works with a maximum of one change. Tapping undo a second time simply "Redo" the undone change.
 2. `Cut File`: Cut a selected text section.
Under the `Edit` tab on the top-left corner, click on the `Cut` button or press `Ctrl-X`on your keyboard.
 3. `Copy`: Copy a selected text section.
Under the `Edit` tab on the top-left corner, click on the `Copy` button or press `Ctrl-C`on your keyboard.
 4. `Paste`: Paste an already-copied text section in.
 Under the `Edit` tab on the top-left corner, click on the `Paste` button or press `Ctrl-V`on your keyboard.

### Search
![4](https://user-images.githubusercontent.com/44689249/119996343-3d3f7000-c001-11eb-8510-9e20f0fabfb1.jpg)
 1. `Find`: Search for a specific string.
Under the `Search` tab on the top-left corner, click on the `Find` button or press `Ctrl-F` on your keyboard.
 2. `Find Again`: Search for the previously searched string again.
Under the `Search` tab on the top-left corner, click on the `Find Again` button or press `Ctrl-G`on your keyboard.
 3. `Replace`: Replace a specified text with another text of your choosing.
Under the `Search` tab on the top-left corner, click on the `Replace` button or press `Ctrl-R`on your keyboard.
 4. `Replace Again`: Replace a specified text with the last replace parameters.
 Under the `Search` tab on the top-left corner, click on the `Replace Again` button or press `Ctrl-T`on your keyboard.

> Please highlight the section of text you want the operation to work for before using any of the "Search Features"
### Syntax Support
![5](https://user-images.githubusercontent.com/44689249/119998237-1aae5680-c003-11eb-848a-d0be076b8f5f.jpg)

 1. Code Keywords: C/C++ code keywords will be highlighted in **blue**
 2. Type Keywords: C/C++ type keywords will be highlighted in **red** 

> All supported words are shown in the image above. Only C/C++ support for now.

## Edit Source Code

The text editor application was built using Visual Studio 2017 and the FLTK (Version 1.3.4) api was called for the build. You can download FLTK at this [link](https://www.fltk.org/software.php) and you can follow this [guide](https://bumpyroadtocode.com/2017/08/05/how-to-install-and-use-fltk-1-3-4-in-visual-studio-2017-complete-guide/) for the installation process on visual studio 2017. 

> Please follow the guide step-by-step until: ` 3. Go to: the fluid folder in your fltk source folder` as the following steps of the guide has already been incorporated in my project

### File Locations
Using Visual Studio, open `Project1.sln` found in the repository. The source codes are found in `/Project` directory of the repository. Do not change the project properties as it will lead to unwanted behavior. You should either use `Debug` mode or `Release` mode when running the project. 

You will find the main method stored in `/Project1/fltk_editor.cpp` and the global variables stored in `/Project1/EditorWindow.cpp`
