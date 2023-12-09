#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Database.h"
#include "SecondWindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QGroupBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void importFile();
    void exportFile();
    void resetDB();
    void changeDB();
    void toListProfs();
    void toListProfils();
    void toListCompetence();
    void manageProfils();
    void manageProfs();
    void manageComp();

private:
    static void saveDialog(QWidget *parent, QString title, QString labelText, bool *ok, bool *pro, bool *pfl, bool *cpt);

    QPushButton *importButton, *exportButton, *reinitDBButton, *changerDBButton;
    QPushButton *listerProfButton, *listerProButton, *listerComButton;
    QPushButton *profButton, *profilButton, *competenceButton;
    QGroupBox *dataGroup, *fastActionGroup, *manageGroup;

    Database *QtDB;
};
#endif // MAINWINDOW_H
