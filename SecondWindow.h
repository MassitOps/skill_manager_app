#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include "Database.h"
#include <QDialog>
#include <QString>
#include <QPushButton>
#include <QStringList>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class SecondWindow : public QDialog
{
    Q_OBJECT
public:
    SecondWindow(QStringList& list, QString& title, QWidget *parent = nullptr);
    SecondWindow(Database *db, QStringList& profilList, QWidget *parent = nullptr);
    SecondWindow(Database *db, QStringList& listN, QStringList& listP, QWidget *parent = nullptr);
    SecondWindow(Database *db, QWidget *parent = nullptr);
    ~SecondWindow();

public slots:
    void refreshProfiles(QString word);
    void refreshComps(QString word);
    void refreshComps();
    void refreshProfs(QString word);
    void refreshProfs();
    void addProfil();
    void addSkill();
    void describeProfile();
    void renameProfil();
    void removeProfil();
    void removeSkill();
    void refreshInfos();

    void refreshInfosP();
    void refreshProfsP(QString word);
    void refreshCompsP(QString word);
    void refreshCompsP();
    void refreshProfilesP(QString word);
    void refreshProfilesP();
    void addSkillP();
    void removeSkillP();
    void removeProfP();
    void addProfP();
    void renameProfP();

    void refreshCompsC(QString word);
    void removeSkillC();
    void addSkillC();
    void renameSkillC();

private:
    Database *m_db;

    QString current, currentNom, currentPrenom;
    QPushButton *addProfilButton, *removeProfilButton, *descProfilButton, *renameProfilButton;
    QPushButton *addCompButton, *removeCompButton, *quitButton;
    QListWidget *listWidget, *compListWidget, *profListWidget;
    QLineEdit *lineEdit, *compLineEdit, *profLineEdit;

    static void getPro(QWidget *parent, QString title, QString labelText, bool *ok, QString *nom, QString *prenom);
    static void getDesc(QWidget *parent, QString title, QString labelText, QString description, bool *ok, QString *desc);
};

#endif // SECONDWINDOW_H
