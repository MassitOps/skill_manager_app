#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QString>
#include <QStringList>

class Database : public QWidget
{
    Q_OBJECT

public:
    Database(const QString& chemin = "../GestionCompetences/QtDB.db");
    ~Database();

    void createDB(const QString& chemin = "QtDB.db");
    bool reinitialiserDB();
    void openDB(const QString& chemin);
    void closeDB();

    int addCompetence(const QString& competence);
    int addProfil(const QString& profil);
    int addProfessionnel(const QString& nom, const QString& prenom);
    bool addCompToProfil(const int idProfil, const int idCompetence);
    bool addCompToProfil(const QString& profil, const QString& competence);
    bool addCompToProf(const int idProfessionnel, const int idCompetence);
    bool addCompToProf(const QString& nom, const QString& prenom, const QString& competence);

    bool addDescToProfil(const QString& profil, const QString& description);
    QString getDescFromProfil(const QString& profil);

    bool chargerFichier(const QString& chemin, QWidget *parent = nullptr);
    bool exporterVers(const QString& chemin, bool pro=true, bool pfl=true, bool cpt=true);

    bool removeComFromProfil(const QString& profil, const QString& competence);
    bool removeComFromProf(const QString& nom, const QString& prenom, const QString& competence);
    bool removeProfil(const QString& profil);
    bool removeProfessionnel(const QString& nom, const QString& prenom);
    bool removeCompetence(const QString& competence);

    bool renameCompetence(const QString& oldCom, const QString& newCom);
    bool renameProfil(const QString& oldPro, const QString& newPro);
    bool renameProfessionnel(const QString& oldNom, const QString& oldPrenom, const QString& newNom, const QString& newPrenom);

    QStringList listerCompetences();
    QStringList listerProfils();
    void listerProfessionnels(QStringList& listeNProf, QStringList& listePProf, bool sorted = false);
    QStringList listerComProfil(const QString& nom);
    QStringList listerComProf(const QString& nom, const QString& prenom);
    QStringList listerProfilsProf(const QString& nom, const QString& prenom);
    void listerProfsProfil(const QString& nom, QStringList& listNProfsProfil, QStringList& listPProfsProfil);

    static void strip(QString& mot);

private:
    static void capitalize(QString &mot);
    bool contains(const QStringList& big, const QStringList& small);

    QSqlDatabase db;
};

#endif // DATABASE_H
