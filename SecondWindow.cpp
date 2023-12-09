#include "SecondWindow.h"
#include <QListView>
#include <QStringListModel>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QDebug>

SecondWindow::SecondWindow(QStringList& list, QString& title, QWidget *parent) : QDialog(parent)
{
    QStringListModel *model = new QStringListModel(list);

    QListView *listView = new QListView();
    listView->setModel(model);
    listView->setSelectionMode(QAbstractItemView::NoSelection);
    listView->setFont(QFont("MS Shell Dlg", 11));
    QPushButton *fermerButton = new QPushButton("Fermer");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(listView);
    mainLayout->addWidget(fermerButton);

    setWindowTitle(title);
    setMinimumSize(320, 350);
    setLayout(mainLayout);
    setModal(true);

    QObject::connect(fermerButton, SIGNAL(clicked()), this, SLOT(close()));
}

SecondWindow::SecondWindow(Database* db, QStringList& profilList, QWidget *parent) : QDialog(parent)
{
    m_db = db;

    lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("Rechercher un profil");

    QLabel *label = new QLabel("Liste des\n  profils");
    label->setWordWrap(true);
    label->setFont(QFont("MS Shell Dlg", 8, QFont::Bold));

    listWidget = new QListWidget;
    listWidget->addItems(profilList);

        descProfilButton = new QPushButton("Description");
        descProfilButton->setToolTip("Voir la description du profil sélectionné");
        descProfilButton->setToolTipDuration(5000);
        descProfilButton->setMinimumSize(10, 10);

        addProfilButton = new QPushButton("Créer");
        addProfilButton->setToolTip("Créer un nouveau profil");
        addProfilButton->setToolTipDuration(5000);
        addProfilButton->setMinimumSize(10, 10);

        renameProfilButton = new QPushButton("Rename");
        renameProfilButton->setToolTip("Renommer le profil sélectionné");
        renameProfilButton->setToolTipDuration(5000);
        renameProfilButton->setFont(QFont("MS Shell Dlg", 8));
        renameProfilButton->setMinimumSize(10, 10);

        removeProfilButton = new QPushButton("Delete");
        removeProfilButton->setToolTip("Supprimer le prolfil sélectionné");
        removeProfilButton->setToolTipDuration(5000);
        removeProfilButton->setMinimumSize(10, 10);

        QHBoxLayout *miniLayout00 = new QHBoxLayout;
        miniLayout00->addWidget(label);
        miniLayout00->addWidget(descProfilButton);

        QHBoxLayout *miniLayout0 = new QHBoxLayout;
        miniLayout0->addWidget(addProfilButton);
        miniLayout0->addWidget(renameProfilButton);
        miniLayout0->addWidget(removeProfilButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(lineEdit);
    layout->addLayout(miniLayout00);
    layout->addWidget(listWidget);
    layout->addLayout(miniLayout0);

    compLineEdit = new QLineEdit;
    compLineEdit->setPlaceholderText("Rechercher une competence");

    QLabel *compLabel = new QLabel("Liste des compétences du profil sélectionné");
    compLabel->setWordWrap(true);
    compLabel->setFont(QFont("MS Shell Dlg", 8, QFont::Bold));

    compListWidget = new QListWidget;
        QStringList list;
    compListWidget->addItems(list);

        addCompButton = new QPushButton("Ajouter");
        addCompButton->setToolTip("Ajouter un nouvelle compétence au profil");
        addCompButton->setToolTipDuration(5000);

        removeCompButton = new QPushButton("Retirer");
        removeCompButton->setToolTip("Retirer la compétence sélectionnée du profil");
        removeCompButton->setToolTipDuration(5000);

        QHBoxLayout *miniLayout = new QHBoxLayout;
        miniLayout->addWidget(addCompButton);
        miniLayout->addWidget(removeCompButton);

    QVBoxLayout *compLayout = new QVBoxLayout;
    compLayout->addWidget(compLineEdit);
    compLayout->addWidget(compLabel);
    compLayout->addWidget(compListWidget);
    compLayout->addLayout(miniLayout);

    profLineEdit = new QLineEdit;
    profLineEdit->setPlaceholderText("Rechercher un professionnel");

    QLabel *profsLabel = new QLabel("Liste des professionnels ayant ce profil");
    profsLabel->setWordWrap(true);
    profsLabel->setFont(QFont("MS Shell Dlg", 8, QFont::Bold));

    profListWidget = new QListWidget;
    profListWidget->addItems(list);

        quitButton = new QPushButton("Fermer");

    QVBoxLayout *profsLayout = new QVBoxLayout;
    profsLayout->addWidget(profLineEdit);
    profsLayout->addWidget(profsLabel);
    profsLayout->addWidget(profListWidget);
    profsLayout->addWidget(quitButton);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addLayout(compLayout);
    mainLayout->addLayout(profsLayout);

    setMaximumSize(500, 350);
    setFont(QFont("MS Shell Dlg", 10));
    setWindowTitle("Gérer les Profils");
    setLayout(mainLayout);
    setModal(true);

    QObject::connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(refreshProfiles(QString)));
    QObject::connect(compLineEdit, SIGNAL(textChanged(QString)), this, SLOT(refreshComps(QString)));
    QObject::connect(profLineEdit, SIGNAL(textChanged(QString)), this, SLOT(refreshProfs(QString)));
    QObject::connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(refreshInfos()));
    QObject::connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(refreshComps()));
    QObject::connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(refreshProfs()));
    QObject::connect(addProfilButton, SIGNAL(clicked()), this, SLOT(addProfil()));
    QObject::connect(descProfilButton, SIGNAL(clicked()), this, SLOT(describeProfile()));
    QObject::connect(renameProfilButton, SIGNAL(clicked()), this, SLOT(renameProfil()));
    QObject::connect(removeProfilButton, SIGNAL(clicked()), this, SLOT(removeProfil()));
    QObject::connect(addCompButton, SIGNAL(clicked()), this, SLOT(addSkill()));
    QObject::connect(removeCompButton, SIGNAL(clicked()), this, SLOT(removeSkill()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

SecondWindow::SecondWindow(Database *db, QStringList& listN, QStringList& listP, QWidget *parent) : QDialog(parent)
{
    m_db = db;
    QStringList listProfs;
    int n = listN.size();
    for(int i=0;i<n;i++) listProfs << listN[i] + " " + listP[i];

    lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("Rechercher un professionnel");

    QLabel *label = new QLabel("\n  Liste des professionnels");
    label->setWordWrap(true);
    label->setFont(QFont("MS Shell Dlg", 8, QFont::Bold));

    listWidget = new QListWidget;
    listWidget->addItems(listProfs);

        addProfilButton = new QPushButton("Add");
        addProfilButton->setToolTip("Créer un nouveau professionnel");
        addProfilButton->setToolTipDuration(5000);
        addProfilButton->setMinimumSize(10, 10);

        renameProfilButton = new QPushButton("Rename");
        renameProfilButton->setToolTip("Renommer le professionnel sélectionné");
        renameProfilButton->setToolTipDuration(5000);
        renameProfilButton->setFont(QFont("MS Shell Dlg", 8));
        renameProfilButton->setMinimumSize(10, 10);

        removeProfilButton = new QPushButton("Delete");
        removeProfilButton->setToolTip("Supprimer le professionnel sélectionné");
        removeProfilButton->setToolTipDuration(5000);
        removeProfilButton->setMinimumSize(10, 10);

        QHBoxLayout *miniLayout0 = new QHBoxLayout;
        miniLayout0->addWidget(addProfilButton);
        miniLayout0->addWidget(renameProfilButton);
        miniLayout0->addWidget(removeProfilButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(label);
    layout->addWidget(listWidget);
    layout->addLayout(miniLayout0);

    compLineEdit = new QLineEdit;
    compLineEdit->setPlaceholderText("Rechercher une competence");

    QLabel *compLabel = new QLabel("Liste des compétences du professionnel sélectionné");
    compLabel->setWordWrap(true);
    compLabel->setFont(QFont("MS Shell Dlg", 8, QFont::Bold));

    compListWidget = new QListWidget;
        QStringList list;
    compListWidget->addItems(list);

        addCompButton = new QPushButton("Ajouter");
        addCompButton->setToolTip("Ajouter un nouvelle compétence au professionnel");
        addCompButton->setToolTipDuration(5000);

        removeCompButton = new QPushButton("Retirer");
        removeCompButton->setToolTip("Retirer la compétence sélectionnée du professionnel");
        removeCompButton->setToolTipDuration(5000);

        QHBoxLayout *miniLayout = new QHBoxLayout;
        miniLayout->addWidget(addCompButton);
        miniLayout->addWidget(removeCompButton);

    QVBoxLayout *compLayout = new QVBoxLayout;
    compLayout->addWidget(compLineEdit);
    compLayout->addWidget(compLabel);
    compLayout->addWidget(compListWidget);
    compLayout->addLayout(miniLayout);

    profLineEdit = new QLineEdit;
    profLineEdit->setPlaceholderText("Rechercher un profil");

    QLabel *profsLabel = new QLabel("Liste des profils de ce professionnel");
    profsLabel->setWordWrap(true);
    profsLabel->setFont(QFont("MS Shell Dlg", 8, QFont::Bold));

    profListWidget = new QListWidget;
    profListWidget->addItems(list);

        quitButton = new QPushButton("Fermer");

    QVBoxLayout *profsLayout = new QVBoxLayout;
    profsLayout->addWidget(profLineEdit);
    profsLayout->addWidget(profsLabel);
    profsLayout->addWidget(profListWidget);
    profsLayout->addWidget(quitButton);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addLayout(compLayout);
    mainLayout->addLayout(profsLayout);

    setMaximumSize(500, 350);
    setFont(QFont("MS Shell Dlg", 10));
    setWindowTitle("Gérer les Professionnels");
    setLayout(mainLayout);
    setModal(true);

    QObject::connect(profLineEdit, SIGNAL(textChanged(QString)), this, SLOT(refreshProfilesP(QString)));
    QObject::connect(compLineEdit, SIGNAL(textChanged(QString)), this, SLOT(refreshCompsP(QString)));
    QObject::connect(addCompButton, SIGNAL(clicked()), this, SLOT(addSkillP()));
    QObject::connect(removeCompButton, SIGNAL(clicked()), this, SLOT(removeSkillP()));
    QObject::connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(refreshProfsP(QString)));
    QObject::connect(addProfilButton, SIGNAL(clicked()), this, SLOT(addProfP()));
    QObject::connect(renameProfilButton, SIGNAL(clicked()), this, SLOT(renameProfP()));
    QObject::connect(removeProfilButton, SIGNAL(clicked()), this, SLOT(removeProfP()));
    QObject::connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(refreshInfosP()));
    QObject::connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(refreshProfilesP()));
    QObject::connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(refreshCompsP()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

SecondWindow::SecondWindow(Database *db, QWidget *parent) : QDialog(parent)
{
    m_db = db;

    QStringList list = m_db->listerCompetences();
    list.sort();

    lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("Rechercher une compétence");

    QLabel *label = new QLabel("Liste des compétences");
    label->setWordWrap(true);
    label->setFont(QFont("MS Shell Dlg", 8, QFont::Bold));

    listWidget = new QListWidget;
    listWidget->addItems(list);

    addProfilButton = new QPushButton("Créer");
    addProfilButton->setToolTip("Créer une nouvelle compétence");
    addProfilButton->setToolTipDuration(5000);
    addProfilButton->setMinimumSize(10, 10);

    renameProfilButton = new QPushButton("Rename");
    renameProfilButton->setToolTip("Renommer la compétence sélectionnée");
    renameProfilButton->setToolTipDuration(5000);
    renameProfilButton->setMinimumSize(10, 10);

    removeProfilButton = new QPushButton("Delete");
    removeProfilButton->setToolTip("Supprimer la compétence sélectionnée");
    removeProfilButton->setToolTipDuration(5000);
    removeProfilButton->setMinimumSize(10, 10);

    quitButton = new QPushButton("Fermer");
    quitButton->setMinimumSize(10, 10);

    QHBoxLayout *subLayout = new QHBoxLayout;
    subLayout->addWidget(addProfilButton);
    subLayout->addWidget(renameProfilButton);
    subLayout->addWidget(removeProfilButton);
    subLayout->addWidget(quitButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(label);
    layout->addWidget(listWidget);
    layout->addLayout(subLayout);

    setFixedSize(280, 350);
    setFont(QFont("MS Shell Dlg", 10));
    setWindowTitle("Gérer les Compétences");
    setLayout(layout);
    setModal(true);

    QObject::connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(refreshCompsC(QString)));
    QObject::connect(addProfilButton, SIGNAL(clicked()), this, SLOT(addSkillC()));
    QObject::connect(renameProfilButton, SIGNAL(clicked()), this, SLOT(renameSkillC()));
    QObject::connect(removeProfilButton, SIGNAL(clicked()), this, SLOT(removeSkillC()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

SecondWindow::~SecondWindow()
{
}

void SecondWindow::refreshInfos()
{
    QListWidgetItem *item = new QListWidgetItem;
    item = listWidget->currentItem();
    if(item)
    {
        current = item->text();
        compListWidget->clear();
        QStringList list = m_db->listerComProfil(current);
        list.sort();
        compListWidget->addItems(list);

        profListWidget->clear();
        QStringList listN, listP;
        list.clear();
        m_db->listerProfsProfil(current, listN, listP);
        int n = listN.size();
        for(int i=0;i<n;i++) list << listN[i] + " " + listP[i];
        list.sort();
        profListWidget->addItems(list);
    }
}

void SecondWindow::refreshProfiles(QString word)
{
    compListWidget->clear();
    profListWidget->clear();

    QStringList list, baseList;
    baseList = m_db->listerProfils();
    baseList.sort();
    list = baseList.filter(word, Qt::CaseInsensitive);
    listWidget->clear();
    listWidget->addItems(list);
}

void SecondWindow::refreshComps(QString word)
{
    QStringList list, baseList;
    baseList = m_db->listerComProfil(current);
    baseList.sort();
    list = baseList.filter(word, Qt::CaseInsensitive);
    compListWidget->clear();
    compListWidget->addItems(list);
}

void SecondWindow::refreshComps()
{
    refreshComps(compLineEdit->text());
}

void SecondWindow::refreshProfs(QString word)
{
    QStringList list, baseList, listN, listP;
    m_db->listerProfsProfil(current, listN, listP);
    int n = listN.size();
    for(int i=0;i<n;i++) baseList << listN[i] + " " + listP[i];
    baseList.sort();
    list = baseList.filter(word, Qt::CaseInsensitive);
    profListWidget->clear();
    profListWidget->addItems(list);
}

void SecondWindow::refreshProfs()
{
    refreshProfs(profLineEdit->text());
}

void SecondWindow::removeSkill()
{
    if(compListWidget->selectedItems().isEmpty()) QMessageBox::warning(this, "No Skill Selected", "Échec ! Veuillez sélectionner<br/>une compétence");
    else
    {
        QString skill = compListWidget->currentItem()->text();
        QString msg = "Retirer <b>" + skill + "</b> de la liste de <br/> compétences de <b>" + current + "</b> ?" ;
        int reponse = QMessageBox::question(this, "Supprimer ?", msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(reponse == QMessageBox::Yes)
        {
            if(m_db->removeComFromProfil(current, skill))
            {
                refreshComps();
                refreshProfs();
                QMessageBox::information(this, "Remove " + skill, "<b>Succès</b>");
            }
            else QMessageBox::warning(this, "Remove " + skill, "<b>Échec !</b>");
        }
    }
}

void SecondWindow::addSkill()
{
    bool ok;
    QString skill = QInputDialog::getText(this, "Add Skill ?!", "Nom de la compétence à ajouter à <b>"+current+"</b>:", QLineEdit::Normal, "", &ok);
    if (ok && !skill.isEmpty())
    {
        Database::strip(skill);
        if(m_db->addCompToProfil(current, skill))
        {
            refreshComps();
            refreshProfs();
            QMessageBox::information(this, "Add " + skill, "<b>Succès</b>");
        }
        else QMessageBox::warning(this, "Add " + skill, "<b>Échec</b>");
    }
    else QMessageBox::warning(this, "Add " + skill, "<b>Échec</b>");
}

void SecondWindow::removeProfil()
{
    QString msg = "Supprimer <b>" + current + "</b> des profils";
    int reponse = QMessageBox::question(this, "Supprimer ?", msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(reponse == QMessageBox::Yes)
    {
        if(m_db->removeProfil(current))
        {
            refreshProfiles("");
            compListWidget->clear();
            profListWidget->clear();
            QMessageBox::information(this, "Remove " + current, "<b>Succès</b>");
        }
        else QMessageBox::warning(this, "Remove " + current, "<b>Échec !</b>");
    }
}

void SecondWindow::addProfil()
{
    bool ok;
    QString profil = QInputDialog::getText(this, "Add Profile ?!", "Nom du profil à créer:", QLineEdit::Normal, "", &ok);
    if (ok && !profil.isEmpty())
    {
        Database::strip(profil);
        m_db->addProfil(profil);
        refreshProfiles("");
        QMessageBox::information(this, "Create " + profil, "<b>Succès</b>");
    }
    else QMessageBox::warning(this, "Create " + profil, "<b>Échec</b>");
}

void SecondWindow::describeProfile()
{
    bool ok;
    QString description = m_db->getDescFromProfil(current);
    QString msg = "Voici la description de <b>" + current + "</b><br/>Cliquer <b>Ok</b> pour mettre le nouveau texte<br/>";
    msg += "en tant que description ou <b>Cancel</b> pour annuler";
    QString desc;
    getDesc(this, "Description", msg, description, &ok, &desc);
    if (ok)
    {
        for(int i=0;i<desc.size();i++) if(desc[i]=='\n') desc[i] = ' ';
        Database::strip(desc);
        if(m_db->addDescToProfil(current, desc)) QMessageBox::information(this, "Description", "<b>Succès</b>");
        else QMessageBox::warning(this, "Description", "<b>Échec</b>");
    }
}

void SecondWindow::renameProfil()
{
    if(listWidget->selectedItems().isEmpty()) QMessageBox::warning(this, "No Profil Selected", "Échec ! Veuillez sélectionner<br/>un profil");
    else
    {
        bool ok;
        QString profil = QInputDialog::getText(this, "Nouveau Nom", "Nouveau nom de <b>"+current+"</b>:", QLineEdit::Normal, "", &ok);
        if (ok && !profil.isEmpty())
        {
            Database::strip(profil);
            if(m_db->renameProfil(current, profil))
            {
                refreshProfiles(lineEdit->text());
                compListWidget->clear();
                profListWidget->clear();
                QMessageBox::information(this, "Renommer", "<b>Succès</b>");
            }
            else QMessageBox::warning(this, "Renommer", "<b>Échec</b>");
        }
        else QMessageBox::warning(this, "Renommer", "<b>Échec</b>");
    }
}

void SecondWindow::getDesc(QWidget *parent, QString title, QString labelText, QString description, bool *ok, QString *desc)
{
    *ok = false;

    QLabel *label = new QLabel(labelText);

    QTextEdit *descEdit = new QTextEdit;
    descEdit->setPlainText(description);
    descEdit->moveCursor(QTextCursor::EndOfBlock);
    descEdit->setFixedHeight(80);

    QPushButton *okButton = new QPushButton("Ok");
    QPushButton *cancelButton = new QPushButton("Cancel");
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->setAlignment(Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->addWidget(descEdit);
    mainLayout->addLayout(buttonLayout);

    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle(title);
    dialog->setLayout(mainLayout);
    dialog->setModal(true);

    QObject::connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(close()));
    QObject::connect(okButton, &QPushButton::clicked, [=] () {
        *ok = true;
        *desc = descEdit->toPlainText();
        dialog->close();
    });

    dialog->exec();
}


void SecondWindow::refreshInfosP()
{
    QListWidgetItem *item = new QListWidgetItem;
    item = listWidget->currentItem();
    if(item)
    {
        current = item->text();

        QStringList listN, listP;
        m_db->listerProfessionnels(listN, listP, true);
        int n = listN.size();
        for(int i=0;i<n;i++)
        {
            if(current.contains(listN[i]) && current.contains(listP[i]))
            {
                currentNom = listN[i];
                currentPrenom = listP[i];
                break;
            }
        }

        compListWidget->clear();
        QStringList list = m_db->listerComProf(currentNom, currentPrenom);
        list.sort();
        compListWidget->addItems(list);

        profListWidget->clear();
        list.clear();
        list = m_db->listerProfilsProf(currentNom, currentPrenom);
        list.sort();
        profListWidget->addItems(list);
    }
}

void SecondWindow::refreshProfsP(QString word)
{
    compListWidget->clear();
    profListWidget->clear();

    QStringList list, baseList, listN, listP;
    m_db->listerProfessionnels(listN, listP, true);
    int n = listN.size();
    for(int i=0;i<n;i++) baseList << listN[i] + " " + listP[i];

    list = baseList.filter(word, Qt::CaseInsensitive);
    listWidget->clear();
    listWidget->addItems(list);
}

void SecondWindow::refreshCompsP(QString word)
{
    QStringList list, baseList;
    baseList = m_db->listerComProf(currentNom, currentPrenom);
    baseList.sort();
    list = baseList.filter(word, Qt::CaseInsensitive);
    compListWidget->clear();
    compListWidget->addItems(list);
}

void SecondWindow::refreshCompsP()
{
    refreshCompsP(compLineEdit->text());
}

void SecondWindow::refreshProfilesP(QString word)
{
    QStringList list, baseList;
    baseList = m_db->listerProfilsProf(currentNom, currentPrenom);
    baseList.sort();
    list = baseList.filter(word, Qt::CaseInsensitive);
    profListWidget->clear();
    profListWidget->addItems(list);
}

void SecondWindow::refreshProfilesP()
{
    refreshProfilesP(profLineEdit->text());
}

void SecondWindow::removeSkillP()
{
    if(compListWidget->selectedItems().isEmpty()) QMessageBox::warning(this, "No Skill Selected", "Échec ! Veuillez sélectionner<br/>une compétence");
    else
    {
        QString skill = compListWidget->currentItem()->text();
        QString msg = "Retirer <b>" + skill + "</b> de la liste de <br/> compétences de <b>" + current + "</b> ?" ;
        int reponse = QMessageBox::question(this, "Supprimer ?", msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(reponse == QMessageBox::Yes)
        {
            if(m_db->removeComFromProf(currentNom, currentPrenom, skill))
            {
                refreshCompsP();
                refreshProfilesP();
                QMessageBox::information(this, "Remove " + skill, "<b>Succès</b>");
            }
            else QMessageBox::warning(this, "Remove " + skill, "<b>Échec !</b>");
        }
    }
}

void SecondWindow::addSkillP()
{
    bool ok;
    QString skill = QInputDialog::getText(this, "Add Skill ?!", "Nom de la compétence à ajouter à <b>"+current+"</b>:", QLineEdit::Normal, "", &ok);
    if (ok && !skill.isEmpty())
    {
        Database::strip(skill);
        if(m_db->addCompToProf(currentNom, currentPrenom, skill))
        {
            refreshCompsP();
            refreshProfilesP();
            QMessageBox::information(this, "Add " + skill, "<b>Succès</b>");
        }
        else QMessageBox::warning(this, "Add " + skill, "<b>Échec</b>");
    }
    else QMessageBox::warning(this, "Add " + skill, "<b>Échec</b>");
}

void SecondWindow::removeProfP()
{
    QString msg = "Supprimer <b>" + current + "</b> des professionnels";
    int reponse = QMessageBox::question(this, "Supprimer ?", msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(reponse == QMessageBox::Yes)
    {
        if(m_db->removeProfessionnel(currentNom, currentPrenom))
        {
            refreshProfsP("");
            compListWidget->clear();
            profListWidget->clear();
            QMessageBox::information(this, "Remove " + current, "<b>Succès</b>");
        }
        else QMessageBox::warning(this, "Remove " + current, "<b>Échec !</b>");
    }
}

void SecondWindow::addProfP()
{
    bool ok;
    QString nom, prenom;
    getPro(this, "Add Professional ?!", "Nom et prénom du professionnel à ajouter:", &ok, &nom, &prenom);
    if (ok && !nom.isEmpty() && !prenom.isEmpty())
    {
        Database::strip(nom);
        Database::strip(prenom);
        m_db->addProfessionnel(nom, prenom);
        refreshProfsP("");
        QMessageBox::information(this, "Create " + nom + " " + prenom, "<b>Succès</b>");
    }
    else QMessageBox::warning(this, "Create " + nom + " " + prenom, "<b>Échec</b>");
}

void SecondWindow::renameProfP()
{
    if(listWidget->selectedItems().isEmpty()) QMessageBox::warning(this, "No Professional Selected", "Échec ! Veuillez sélectionner<br/>un professionnel");
    else
    {
        bool ok;
        QString nom, prenom;
        getPro(this, "Nouveau Nom", "Nouveaux nom et prénom de <b>"+current+"</b>:", &ok, &nom, &prenom);
        if (ok && !nom.isEmpty() && !prenom.isEmpty())
        {
            Database::strip(nom);
            Database::strip(prenom);
            if(m_db->renameProfessionnel(currentNom, currentPrenom, nom, prenom))
            {
                refreshProfsP(lineEdit->text());
                compListWidget->clear();
                profListWidget->clear();
                QMessageBox::information(this, "Renommer", "<b>Succès</b>");
            }
            else QMessageBox::warning(this, "Renommer", "<b>Échec</b>");
        }
        else QMessageBox::warning(this, "Renommer", "<b>Échec</b>");
    }
}


void SecondWindow::refreshCompsC(QString word)
{
    QStringList list, baseList;
    baseList = m_db->listerCompetences();
    baseList.sort();
    list = baseList.filter(word, Qt::CaseInsensitive);
    listWidget->clear();
    listWidget->addItems(list);
}

void SecondWindow::removeSkillC()
{
    if(listWidget->selectedItems().isEmpty()) QMessageBox::warning(this, "No Skill Selected", "Échec ! Veuillez sélectionner<br/>une compétence");
    else
    {
        QString skill = listWidget->currentItem()->text();
        QString msg = "Retirer <b>" + skill + "</b> des compétences ?" ;
        int reponse = QMessageBox::question(this, "Supprimer ?", msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(reponse == QMessageBox::Yes)
        {
            if(m_db->removeCompetence(skill))
            {
                refreshCompsC(lineEdit->text());
                QMessageBox::information(this, "Remove " + skill, "<b>Succès</b>");
            }
            else QMessageBox::warning(this, "Remove " + skill, "<b>Échec !</b>");
        }
    }
}

void SecondWindow::addSkillC()
{
    bool ok;
    QString skill = QInputDialog::getText(this, "Add Skill ?!", "Nom de la compétence à ajouter:", QLineEdit::Normal, "", &ok);
    if (ok && !skill.isEmpty())
    {
        Database::strip(skill);
        if(m_db->addCompetence(skill))
        {
            refreshCompsC(lineEdit->text());
            QMessageBox::information(this, "Add " + skill, "<b>Succès</b>");
        }
        else QMessageBox::warning(this, "Add " + skill, "<b>Échec</b>");
    }
    else QMessageBox::warning(this, "Add " + skill, "<b>Échec</b>");
}

void SecondWindow::renameSkillC()
{
    if(listWidget->selectedItems().isEmpty()) QMessageBox::warning(this, "No Skill Selected", "Échec ! Veuillez sélectionner<br/>une compétence");
    else
    {
        current = listWidget->currentItem()->text();
        bool ok;
        QString skill = QInputDialog::getText(this, "Nouveau Nom", "Nouveau nom de <b>"+current+"</b>:", QLineEdit::Normal, "", &ok);
        if (ok && !skill.isEmpty())
        {
            Database::strip(skill);
            if(m_db->renameCompetence(current, skill))
            {
                refreshCompsC(lineEdit->text());
                QMessageBox::information(this, "Renommer", "<b>Succès</b>");
            }
            else QMessageBox::warning(this, "Renommer", "<b>Échec</b>");
        }
        else QMessageBox::warning(this, "Renommer", "<b>Échec</b>");
    }
}


void SecondWindow::getPro(QWidget *parent, QString title, QString labelText, bool *ok, QString *nom, QString *prenom)
{
    *ok = false;

    QLabel *label = new QLabel(labelText);

    QLineEdit *nEdit = new QLineEdit;
    QLineEdit *pEdit = new QLineEdit;
    QFormLayout *proLayout = new QFormLayout;
    proLayout->addRow(tr("&Nom:"), nEdit);
    proLayout->addRow(tr("&Prénom:"), pEdit);

    QPushButton *okButton = new QPushButton("Ok");
    QPushButton *cancelButton = new QPushButton("Cancel");
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->setAlignment(Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->addLayout(proLayout);
    mainLayout->addLayout(buttonLayout);

    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle(title);
    dialog->setLayout(mainLayout);
    dialog->setModal(true);

    QObject::connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(close()));
    QObject::connect(okButton, &QPushButton::clicked, [=] () {
        *ok = true;
        *nom = nEdit->text();
        *prenom = pEdit->text();
        dialog->close();
    });

    dialog->exec();
}
