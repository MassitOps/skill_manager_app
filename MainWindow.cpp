#include "MainWindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    int h = 34;
    importButton = new QPushButton("Importer un fichier");
    importButton->setMinimumHeight(h);
    exportButton = new QPushButton("Exporter Vers");
    exportButton->setMinimumHeight(h);
    reinitDBButton = new QPushButton("Réinitialiser la BD");
    reinitDBButton->setMinimumHeight(h);
    changerDBButton = new QPushButton("Changer la BD");
    changerDBButton->setMinimumHeight(h);

    QVBoxLayout *dataGroupLayout = new QVBoxLayout;
    dataGroupLayout->addWidget(importButton);
    dataGroupLayout->addWidget(exportButton);
    dataGroupLayout->addWidget(reinitDBButton);
    dataGroupLayout->addWidget(changerDBButton);

    dataGroup = new QGroupBox("Données");
    dataGroup->setLayout(dataGroupLayout);

    listerProfButton = new QPushButton("Liste Professionnels");
    listerProfButton->setMinimumHeight(h);
    listerProButton = new QPushButton("Liste Profils");
    listerProButton->setMinimumHeight(h);
    listerComButton = new QPushButton("Liste Compétences");
    listerComButton->setMinimumHeight(h);

    QVBoxLayout *fastActionGroupLayout = new QVBoxLayout;
    fastActionGroupLayout->addWidget(listerProfButton);
    fastActionGroupLayout->addWidget(listerProButton);
    fastActionGroupLayout->addWidget(listerComButton);

    fastActionGroup = new QGroupBox("Actions rapides");
    fastActionGroup->setLayout(fastActionGroupLayout);

    profButton = new QPushButton("Professionnel");
    profButton->setMinimumHeight(h);
    profilButton = new QPushButton("Profil");
    profilButton->setMinimumHeight(h);
    competenceButton = new QPushButton("Competence");
    competenceButton->setMinimumHeight(h);

    QVBoxLayout *manageGroupLayout = new QVBoxLayout;
    manageGroupLayout->addWidget(profButton);
    manageGroupLayout->addWidget(profilButton);
    manageGroupLayout->addWidget(competenceButton);

    manageGroup = new QGroupBox("Gérer");
    manageGroup->setLayout(manageGroupLayout);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(dataGroup, 0, 0, 2, 1);
    mainLayout->addWidget(fastActionGroup, 0, 1);
    mainLayout->addWidget(manageGroup, 0, 2);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);

    QPushButton *closeButton = new QPushButton("Quitter");
    mainLayout->addWidget(closeButton, 1, 1, 1, 2);

    setMinimumSize(450, 250);
    setWindowTitle("Skill Manager");
    setFont(QFont("MS Shell Dlg", 10));
    setCentralWidget(centralWidget);
    setWindowIcon(QIcon("../GestionCompetences/GestionCompetence.ico"));

    QtDB = new Database;

    QObject::connect(importButton, SIGNAL(clicked()), this, SLOT(importFile()));
    QObject::connect(reinitDBButton, SIGNAL(clicked()), this, SLOT(resetDB()));
    QObject::connect(changerDBButton, SIGNAL(clicked()), this, SLOT(changeDB()));
    QObject::connect(exportButton, SIGNAL(clicked()), this, SLOT(exportFile()));
    QObject::connect(listerProfButton, SIGNAL(clicked()), this, SLOT(toListProfs()));
    QObject::connect(listerProButton, SIGNAL(clicked()), this, SLOT(toListProfils()));
    QObject::connect(listerComButton, SIGNAL(clicked()), this, SLOT(toListCompetence()));
    QObject::connect(profilButton, SIGNAL(clicked()), this, SLOT(manageProfils()));
    QObject::connect(profButton, SIGNAL(clicked()), this, SLOT(manageProfs()));
    QObject::connect(competenceButton, SIGNAL(clicked()), this, SLOT(manageComp()));
    QObject::connect(closeButton, SIGNAL(clicked()), qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::importFile()
{
    QString chemin = QFileDialog::getOpenFileName(this, tr("Choisir le fichier CPT à importer"), "../", tr("Fichier .cpt (*.cpt)"));
    if(!chemin.isNull())
    {
        QString q = "Vous pouvez soit <b>écraser</b> <br/> la base de données préexixtante <br/> soit lui  <b>ajouter</b> les données à <br/> importer.";
        q += "<br/>Pour l'<b>Écraser</b>, sélectionner <b>Oui</b> <br/> sinon sélectionner <b>Non</b>";
        int reponse = QMessageBox::question(this, "Écraser la BD ?", q,QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(reponse == QMessageBox::Yes) QtDB->reinitialiserDB();
        if(QtDB->chargerFichier(chemin, this))
            QMessageBox::information(this, "Charger fichier", "<b>Fichier chargé avec succès</b>");
    }
    else QMessageBox::warning(this, "Charger fichier", "<b>Fichier non chargé</b>");
}

void MainWindow::resetDB()
{
    int reponse = QMessageBox::question(this, "Réinitialiser BD ?", "<b>Continuer ?</b>", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(reponse == QMessageBox::Yes)
    {
        if(QtDB->reinitialiserDB()) QMessageBox::information(this, "Reset Database", "<b>Succès</b>");
        else QMessageBox::warning(this, "Reset Database", "<b>Échec !</b>");
    }
}

void MainWindow::changeDB()
{
    QString chemin = QFileDialog::getOpenFileName(this, tr("Choisir la nouvelle Base de données"), "../", tr("Fichiers de BDD (*.db *db3 *.bdd *.sqlite *.sqlite3)"));
    if(!chemin.isNull())
    {
        QtDB->closeDB();
        QtDB->openDB(chemin);
        QMessageBox::information(this, "Charger Base de donnée", "<b>Succès</b>");
    }
    else QMessageBox::warning(this, "Changer Base de donnée", "<b>Échec</b>");
}

void MainWindow::exportFile()
{
    QString chemin = QFileDialog::getSaveFileName(this, tr("Savegarder les données"), "../", tr("Fichier .cpt (*.cpt)"));
    if(!chemin.isNull())
    {
        bool ok = false, pro = true, pfl = true, cpt = true;
        saveDialog(this, "Sélection", "Sélectionner les données à sauvegarder", &ok, &pro, &pfl, &cpt);
        if(ok)
        {
            QtDB->exporterVers(chemin, pro, pfl, cpt);
            QMessageBox::information(this, "Sauvegarde", "<b>Succès</b>");
        }
        else QMessageBox::warning(this, "Sauvegarde", "<b>Échec</b>");
    }
    else QMessageBox::warning(this, "Sauvegarde", "<b>Échec</b>");
}

void MainWindow::toListProfs()
{
    QStringList listNProfs, listPProfs, list;
    QtDB->listerProfessionnels(listNProfs, listPProfs);

    int n = listNProfs.size();
    for(int i=0;i<n;i++) list << listNProfs[i] + " " + listPProfs[i];
    list.sort();

    QString title = "Liste des professionnels";
    SecondWindow *win = new SecondWindow(list, title, this);
    win->show();
}

void MainWindow::toListProfils()
{
    QStringList list = QtDB->listerProfils();
    list.sort();
    QString title = "Liste des profils";
    SecondWindow *win = new SecondWindow(list, title, this);
    win->show();
}

void MainWindow::toListCompetence()
{
    QStringList list = QtDB->listerCompetences();
    list.sort();
    QString title = "Liste des compétences";
    SecondWindow *win = new SecondWindow(list, title, this);
    win->show();
}

void MainWindow::manageProfils()
{
    QStringList list;
    list =  QtDB->listerProfils();
    list.sort();
    SecondWindow *win = new SecondWindow(QtDB, list, this);
    win->show();
}

void MainWindow::manageProfs()
{
    QStringList listNProfs, listPProfs;
    QtDB->listerProfessionnels(listNProfs, listPProfs, true);
    SecondWindow *win = new SecondWindow(QtDB, listNProfs, listPProfs, this);
    win->show();
}

void MainWindow::manageComp()
{
    SecondWindow *win = new SecondWindow(QtDB, this);
    win->show();
}


void MainWindow::saveDialog(QWidget *parent, QString title, QString labelText, bool *ok, bool *pro, bool *pfl, bool *cpt)
{
    *ok = false;

    QLabel *label = new QLabel(labelText);

    QCheckBox *checkPro = new QCheckBox("Les Professionnels");
    checkPro->setChecked(true);
    QCheckBox *checkPfl = new QCheckBox("Les Profils");
    checkPfl->setChecked(true);
    QCheckBox *checkCpt = new QCheckBox("Les Compétences");
    checkCpt->setChecked(true);
    QVBoxLayout *checkLayout = new QVBoxLayout;
    checkLayout->addWidget(checkPro);
    checkLayout->addWidget(checkPfl);
    checkLayout->addWidget(checkCpt);
    QPushButton *okButton = new QPushButton("Ok");
    QPushButton *cancelButton = new QPushButton("Cancel");
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->setAlignment(Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->addLayout(checkLayout);
    mainLayout->addLayout(buttonLayout);

    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle(title);
    dialog->setLayout(mainLayout);
    dialog->setModal(true);

    QObject::connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(close()));
    QObject::connect(okButton, &QPushButton::clicked, [=] () {
        *ok = true;
        if(!checkPro->isChecked()) *pro = false;
        if(!checkPfl->isChecked()) *pfl = false;
        if(!checkCpt->isChecked()) *cpt = false;
        dialog->close();
    });

    dialog->exec();
}
