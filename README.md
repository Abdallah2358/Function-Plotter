# [**Function Plotter**](https://github.com/Abdallah2358/Function-Plotter)

## Description:
This is Desktop Application with a simple UI to plot simple functions with support of these operators [ + - * / ^]
Built with QT in C++.


## Getting started
### Requirements and pre-requisites
To effectively initialize this project, Developers needs to have [Qt 5.12](https://www.qt.io/offline-installers) with the suitable Mingw build kit on their machines and [Qt Creator](https://www.qt.io/offline-installers) to build the project.

To open the project in ``Qt Creator`` simply open the [task1.pro](task1.pro).


### Tests
The tests are written using the [**Qt Test**](https://doc.qt.io/qt-5/qttest-index.html) to test helper function of the project. 

The tests are run by using Qt Creator by opening the tst_className.pro from testing project in the [TestingHelperClasses](TestingHelperClasses/) folder.

Example : [test_funcObj.pro](TestingHelperClasses/test_funcObj/test_funcObj.pro)

There are provided 23 tests provided in the [tst_funcobj.cpp](TestingHelperClasses/test_funcObj/tst_funcobj.cpp).
IT is a working progress.

### Project Structure
      │   .gitignore
      │   README.md
      │   task1.pro
      │   task1.pro.user
      │
      ├───HelperClasses
      │       functionobj.cpp
      │       functionobj.h
      │       plot.cpp
      │       plot.h
      │       qcustomplot.cpp
      │       qcustomplot.h
      │
      ├───Main
      │       main.cpp
      │       mainwindow.cpp
      │       mainwindow.h
      │
      ├───TestingHelperClasses
      │   └───test_funcObj
      │           .gitignore
      │           test_funcObj.pro
      │           test_funcObj.pro.user
      │           tst_funcobj.cpp
      │
      └───ui
            mainwindow.ui
            plot.ui

This structure has 2 main folders:

