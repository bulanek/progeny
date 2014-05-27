/**
  @author	Boris Bulanek
  @company National Radiation Protection Institute, Bartoskova 28, 140 00, Praha 4
  @email  boris.bulanek@suro.cz
  @tel   00420 226 518  279
  @date	02/19/13
  */
// =====================================================================================

/*! \mainpage %Progeny Documentation
  \section about About the program

  Program Progeny  is created in order to provide simple and fast radon progeny concentration estimation.

  Program allow user to obtain and store data using <a href="http://www.sqlite.org">Sqlite</a>  database.

  The program uses some additional packages:
    - UI (User Interface) framework <a href="http://qt-project.org/">Qt</a>
    - a package for minimization  <a href="http://www.gnu.org/software/gsl/">GSL</a>
    - collection of <tt>C++</tt> tools <a href="http://www.boost.org/">Boost</a>
    - database software called <a href="http://www.sqlite.org">Sqlite</a>

 \section Installation

  A single executable file \b progeny.exe  is created for Windows (7 or XP) users using
  <a href="http://pic.dhe.ibm.com/infocenter/aix/v7r1/index.jsp?topic=%2Fcom.ibm.aix.prftungd%2Fdoc%2Fprftungd%2Fwhen_dyn_linking_static_linking.htm">static linking</a>.
  Linux users can execute \c progeny.exe using program <a href="http://www.winehq.org/">wine</a>.
  I haven't tried to install the program on Mac but I believe that the installation is similar to installation on Linux.
  \subsection Linux
  All of needed packages have to be in official repositories of your distribution as all of them are open source programs.
For example in case of <a href="https://www.archlinux.org/"> Arch Linux</a> distribution,
you have only to write down command like:\n <tt>sudo pacman -S cmake boost boost-build boost-libs gsl qt sqlite3 sqliteman</tt>.\n
  After installation you have to go to the src directory and  e.g. follow these steps:\n
  <tt>
  mkdir build\n
  cd build\n
  qmake-qt4 ..\n
  make\n
  ./progeny\n
  </tt>
  This approach uses shared library linking where the size of the executable \c alive is considerably smaller
   and you can use all of additional packages mentioned in the beginning of the section.

  \subsection sql_win Sqlite database

  All the information from binary file and some additional parameters can be stored in a simple database called <a href="http://www.sqlite.org">Sqlite</a>.
  There exist graphical tool for working with a Sqlite database called <a href="http://www.sqliteman.com">Sqliteman</a>.


\section running_code Program user guide
\subsection algorithm Algorithm


\subsection running_program Running program

    If you have any question, please hesitate and send me a mail to: boris.bulanek@suro.cz

    */

#include <QApplication>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include "mainwindow.h"
#include	"datahandle.h"
#include <string>

using namespace std;


const string confDataName="progeny.xml";
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DataHandle* _dataHandle=DataHandle::getInstance();
    _dataHandle->getConfigurationData(confDataName);
    QMenu *helpMenu = w.menuBar()->addMenu(QObject::tr("&About"));
    helpMenu->addAction(QObject::tr("About program"), &w, SLOT(about()));
    helpMenu->addAction(QObject::tr("About Qt"), qApp, SLOT(aboutQt()));
    w.show();
    return a.exec();
}
