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

  Program Progeny  is created in order to provide simple and fast radon progeny concentration estimation. From the knowledge of the concentration of radon progenies
  user can estimate the activity of filter or the signal detected from particular detector with known efficiency for given time interval of data aquisition.

  As an input file is used href="http://www.sqlite.org">Sqlite</a>  database.

  The program uses some additional packages:
    - UI (User Interface) framework <a href="http://qt-project.org/">Qt</a>
    - a package for minimization  <a href="http://www.gnu.org/software/gsl/">GSL</a>
    - collection of <tt>C++</tt> tools <a href="http://www.boost.org/">Boost</a>
    - database software <a href="http://www.sqlite.org">Sqlite</a>

 \section Installation

  A single executable file \b progeny.exe  is created for Windows (7 or XP) users using
  static libraries linking.
  \subsection Linux
  All of needed packages have to be in official repositories of your distribution as all of them are open source programs.
For example in case of <a href="https://www.archlinux.org/"> Arch Linux</a> distribution,
you have only to write down command like:\n <tt>sudo pacman -S cmake boost boost-libs gsl qt sqlite3 sqlitebrowser</tt>.\n
  After installation you have to go to the src directory and  e.g. follow these steps:\n
  <tt>
  mkdir build\n
  cd build\n
  qmake(-qt4) ..\n
  make\n
  ./progeny &\n
  </tt>
  Compilation is made using shared libraries. 

  \subsection sql_win Sqlite database

  All the information from binary file and some additional parameters can be stored in a simple database called <a href="http://www.sqlite.org">Sqlite</a>.
  There exist graphical tool for working with a Sqlite database called <a href="https://github.com/sqlitebrowser/sqlitebrowser">Sqlitebrowser</a>.
  Following conventions are used for input sqlite tables:
  Two tables <i>info_1</i> and <i>info_2</i> are used as an input data tables.
  The content of tables entries is following. 
  The content of <i>info_1</i> table are information connected with filtration through filter.
  The content of <i>info_2</i> table are information about filter measurement with particular detector.
    - info_1
        - <tt>filtration_time</tt> - Time of filtration through filter in seconds \n
        - <tt>air_volume</tt> - Amount of air filtered (liters) \n
        - <tt>filter_efficiency</tt> - Efficiency to catch progenies on filter (no distinction between types of progenies) \n
        - <tt>id</tt> - Identification of measurement needed for <i>info_2</i> table with the same table entry \n
        - <tt>measurement_datetime</tt> - Voluntary information about date and time of measurement \n
        .

    - info_2
        - <tt>signal</tt> - Signal (number of events) obtained from detector measurement \n
        - <tt>startTime</tt> - Time of beginning of measurement with particular detector. The time from the end of filtration (in seconds) \n
        - <tt>timeDelta</tt> - Time of measurement with particular detector (in seconds) \n
        - <tt>detector_efficiency</tt> - Efficiency of detecting signal (events) with particular detector for particular signal.. \n
        - <tt>type</tt> - Type of measurement progenies
            - 0 RnA + RnC  (e.g. summary alpha)
            - 1 RnA
            - 2 RnB
            - 3 RnC
            - 4 RnB+RnC (e.g. summary beta)
            .
        - <tt>id</tt> Identification of measurement with particular device for assignment to the <i>info_1</i> table entry with the same <tt>id</tt>
        . 
    . 


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
