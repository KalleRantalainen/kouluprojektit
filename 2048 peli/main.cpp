/*
 COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 Project4: 2048
 File: main.cpp

 Creates the gui with mainwindow.cpp and mainwindow.hh
´
 INFORMATION ABOUT THE PROGRAM: More information about the program
 in the file documentation.txt. From that file can be found what the
 program does and how it works.

 Notes: this file was aoutomatically created by the qtcreator.

 Programmer information:
 -Programmer: Kalle Rantalainen
 -Name: Kalle Rantalainen
 -Student number: 50329870
 -Username: cdkara
 -E-mail: kalle.rantalainen@tuni.fi
*/

#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
