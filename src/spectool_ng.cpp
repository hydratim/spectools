/* Spectools-NG
 *
 *  A simple utility for using various radio spectrum analysers, with a focus on debugging wifi issues.
 *
 * Tim Armstrong/hydratim <tim@plaintextnerds.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Thanks to Mike Kershaw/Dragorn <dragorn@kismetwireless.net> who wrote the original Spectools.
 */

#include <iostream>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
