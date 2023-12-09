#include "Database.h"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QProgressDialog>
#include <QMessageBox>

Database::Database(const QString& chemin)
{
    if(QFileInfo::exists(chemin)) openDB(chemin);
    else createDB("QtDB.db");
}

Database::~Database()
{
    db.close();
    qDebug() << "Base de données fermée";
}

void Database::createDB(const QString& chemin)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(chemin);
    if(!db.open()) qDebug() << "Erreur de création de la base de donnée";
    else
    {
        QSqlQuery request;
        request.exec("CREATE TABLE competence ( idCom INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, nomCom TEXT NOT NULL );");
        request.exec("CREATE TABLE profil ( idPro INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, nomPro TEXT NOT NULL, description TEXT );");
        request.exec("CREATE TABLE professionnel ( idProf INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, nomProf TEXT NOT NULL, prenomProf TEXT NOT NULL );");
        request.exec("CREATE TABLE prof_comp ( idProf INTEGER NOT NULL, idCom INTEGER NOT NULL, PRIMARY KEY (idProf, idCom), FOREIGN KEY (idProf) REFERENCES professionnel(idProf),  FOREIGN KEY (idCom) REFERENCES competence(idCom) );");
        request.exec("CREATE TABLE profil_comp ( idPro INTEGER NOT NULL, idCom INTEGER NOT NULL, PRIMARY KEY (idPro, idCom), FOREIGN KEY (idPro) REFERENCES profil(idPro),  FOREIGN KEY (idCom) REFERENCES competence(idCom) );");

        qDebug() << "Base de données créée et ouverte";
    }
}

bool Database::reinitialiserDB()
{
    bool ans = true;
    QSqlQuery request;
    if(!request.exec("DELETE FROM prof_comp")) ans = false;
    if(!request.exec("DELETE FROM profil_comp")) ans = false;
    if(!request.exec("DELETE FROM professionnel")) ans = false;
    if(!request.exec("DELETE FROM profil")) ans = false;
    if(!request.exec("DELETE FROM competence")) ans = false;
    return ans;
}

void Database::openDB(const QString& chemin)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(chemin);
    if(db.open()) qDebug() << "Base de donnée ouverte";
    else qDebug() << "Erreur d'ouverture de la base de donnée";
}

void Database::closeDB()
{
    db.close();
}

int Database::addCompetence(const QString& competence)
{
    QSqlQuery request;
    int idCompetence;
    request.prepare("SELECT idCom FROM competence WHERE nomCom = :com");
    request.bindValue(":com", competence);
    request.exec();
    if(request.next())
    {
        idCompetence = request.value(0).toInt();
    }
    else
    {
        QSqlQuery query;
        query.prepare("INSERT INTO competence(nomCom) VALUES (:com)");
        query.bindValue(":com", competence);
        query.exec();
        query.exec("SELECT last_insert_rowid()");
        query.last();
        idCompetence = query.value(0).toInt();
    }
    return idCompetence;
}

int Database::addProfil(const QString& profil)
{
    QSqlQuery request;
    int idProfil;
    request.prepare("SELECT idPro FROM profil WHERE nomPro = :pro");
    request.bindValue(":pro", profil);
    request.exec();
    if(request.next())
    {
        idProfil = request.value(0).toInt();
    }
    else
    {
        QSqlQuery query;
        query.prepare("INSERT INTO profil(nomPro) VALUES (:pro)");
        query.bindValue(":pro", profil);
        query.exec();
        query.exec("SELECT last_insert_rowid()");
        query.last();
        idProfil = query.value(0).toInt();
    }
    return idProfil;
}

int Database::addProfessionnel(const QString& nom, const QString& prenom)
{
    QSqlQuery request;
    int idProfessionnel;
    request.prepare("SELECT idProf FROM professionnel WHERE nomProf = :Nprof AND prenomProf = :Pprof");
    request.bindValue(":Nprof", nom);
    request.bindValue(":Pprof", prenom);
    request.exec();
    if(request.next())
    {
        idProfessionnel = request.value(0).toInt();
    }
    else
    {
        QSqlQuery query;
        query.prepare("INSERT INTO professionnel(nomProf, prenomProf) VALUES (:Nprof, :Pprof)");
        query.bindValue(":Nprof", nom);
        query.bindValue(":Pprof", prenom);
        query.exec();
        query.exec("SELECT last_insert_rowid()");
        query.last();
        idProfessionnel = query.value(0).toInt();
    }
    return idProfessionnel;
}

bool Database::addCompToProfil(const int idProfil, const int idCompetence)
{
    QSqlQuery request;
    request.prepare("SELECT idPro, idCom FROM profil_comp WHERE idPro = :pro AND idCom = :com");
    request.bindValue(":pro", idProfil);
    request.bindValue(":com", idCompetence);
    request.exec();
    if(!request.next())
    {
        QSqlQuery query;
        query.prepare("INSERT INTO profil_comp(idPro, idCom) VALUES (:pro, :com)");
        query.bindValue(":pro", idProfil);
        query.bindValue(":com", idCompetence);
        return query.exec();
    }
    return true;
}

bool Database::addCompToProfil(const QString& profil, const QString& competence)
{
    QStringList listProfils = listerProfils();
    if(listProfils.contains(profil))
    {
        int idPro = addProfil(profil);
        QStringList listComPro = listerComProfil(profil);
        if(!listComPro.contains(competence))
        {
            int idCom = addCompetence(competence);
            return addCompToProfil(idPro, idCom);
         }
         else return true;
    }
    else return true;
}

bool Database::addCompToProf(const int idProfessionnel, const int idCompetence)
{
    QSqlQuery request;
    request.prepare("SELECT idProf, idCom FROM prof_comp WHERE idProf = :prof AND idCom = :com");
    request.bindValue(":prof", idProfessionnel);
    request.bindValue(":com", idCompetence);
    request.exec();
    if(!request.next())
    {
        QSqlQuery query;
        query.prepare("INSERT INTO prof_comp(idProf, idCom) VALUES (:prof, :com)");
        query.bindValue(":prof", idProfessionnel);
        query.bindValue(":com", idCompetence);
        return query.exec();
    }
    return true;
}

bool Database::addCompToProf(const QString& nom, const QString& prenom, const QString& competence)
{
    QStringList listNProf, listPProfs;
    listerProfessionnels(listNProf, listPProfs);
    bool contain = false;
    int n = listNProf.size();
    for(int i=0;i<n;i++)
    {
        if(listNProf[i] == nom  && listPProfs[i] == prenom)
        {
            contain = true;
            break;
        }
    }
    if(contain)
    {
        int idProf = addProfessionnel(nom, prenom);
        QStringList listComProf = listerComProf(nom, prenom);
        if(!listComProf.contains(competence))
        {
            int idCom = addCompetence(competence);
            return addCompToProf(idProf, idCom);
         }
         else return true;
    }
    else return true;
}

bool Database::addDescToProfil(const QString& profil, const QString& description)
{
    QSqlQuery request;
    request.prepare("UPDATE profil SET description = :desc WHERE nomPro = :pro");
    request.bindValue(":desc", description);
    request.bindValue(":pro", profil);
    return request.exec();
}

QString Database::getDescFromProfil(const QString& profil)
{
    QString description;
    QSqlQuery request;
    request.prepare("SELECT description FROM profil WHERE nomPro = :pro");
    request.bindValue(":pro", profil);
    request.exec();
    if(request.next()) description = request.value(0).toString();
    return description;
}

bool Database::chargerFichier(const QString& chemin, QWidget *parent)
{
    QFile file(chemin);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString line;

    QTextStream input(&file);
    int lineCount = 0;
    while (!input.atEnd())
    {
        input.readLine();
        lineCount++;
    }

    file.close();

    QProgressDialog progress("Chargement...", "Arrêt", 0, lineCount, parent);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    input.seek(0);
    int progressValue = 0;

    while (!input.atEnd())
    {
        line = input.readLine();
        strip(line);
        progressValue++;
        progress.setValue(progressValue);
        qApp->processEvents();
        if(progress.wasCanceled()) break;

        if(line == "PRO:")
        {
            QString nom, prenom, competence;
            int idProf, times = 1;

            while(line != "")
            {
                line = input.readLine();
                strip(line);
                progressValue++;
                progress.setValue(progressValue);
                qApp->processEvents();
                if(progress.wasCanceled()) break;

                if(line.isEmpty()) break;
                if(times == 1)
                {
                    nom = line;
                    times++;
                }
                else if(times == 2)
                {
                    prenom = line;
                    times++;
                }
                else if(times == 3)
                {
                    idProf = addProfessionnel(nom, prenom);
                    times++;
                }
                else
                {
                    competence = line;
                    addCompToProf(idProf, addCompetence(competence));
                }
            }
        }

        if(line == "PFL:")
        {
            QString nom, competence;
            int idPro, times = 1;

            while(line != "")
            {
                line = input.readLine();
                strip(line);
                progressValue++;
                progress.setValue(progressValue);
                qApp->processEvents();
                if(progress.wasCanceled()) break;

                if(line.isEmpty()) break;
                if(times == 1)
                {
                    nom = line;
                    times++;
                }
                else if(times == 2)
                {
                    idPro = addProfil(nom);
                    times++;
                }
                else
                {
                    competence = line;
                    if(!competence.startsWith("DESC: "))
                        addCompToProfil(idPro, addCompetence(competence));
                    else
                    {
                        competence.remove(0, 6);
                        addDescToProfil(nom, competence);
                    }
                }
            }
        }

        if(line == "CPT:")
        {
            int times = 1;

            while(line != "")
            {
                line = input.readLine();
                strip(line);
                progressValue++;
                progress.setValue(progressValue);
                qApp->processEvents();
                if(progress.wasCanceled()) break;

                if(line.isEmpty()) break;
                if(times == 1) times++;
                else addCompetence(line);
            }
        }
    }
    int retourner = true;
    if(progress.wasCanceled())
    {
        QMessageBox::critical(parent, "Charger fichier", "<b>Chargement arrêté</b>");
        retourner = false;
    }
    progress.close();
    file.close();
    return retourner;
}

bool Database::exporterVers(const QString& chemin, bool pro, bool pfl, bool cpt)
{
    QFile file(chemin);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QStringList listeNProfs, listePProfs, listeProfils, listeCom;
    listerProfessionnels(listeNProfs, listePProfs);
    listeProfils = listerProfils();

    QTextStream output(&file);
    output.setAutoDetectUnicode(true);

    if(pro)
    {
        int n = listeNProfs.size();
        for(int i=0;i<n;i++)
        {
            listeCom = listerComProf(listeNProfs[i], listePProfs[i]);

            output << "PRO:" << endl;
            output << "\t" << listeNProfs[i] << endl;
            output << "\t" << listePProfs[i] << endl;
            output << "\t" << listeCom.size() << endl;
            for(const QString& com : listeCom)
            {
                output << "\t" << com << endl;
            }
            output << endl;
        }
    }

    if(pfl)
    {
        for(const QString& profil : listeProfils)
        {
            listeCom = listerComProfil(profil);

            output << "PFL:" << endl;
            output << "\t" << profil << endl;
            output << "\t" << listeCom.size() << endl;
            for(const QString& com : listeCom)
            {
                output << "\t" << com << endl;
            }
            QString desc = getDescFromProfil(profil);
            if(!desc.isEmpty()) output << "\tDESC: " << desc << endl;
            output << endl;
        }
    }

    if(cpt)
    {
        listeCom.clear();
        QSqlQuery request;
        if(pro && pfl) request.exec("SELECT competence.nomCom FROM competence LEFT JOIN prof_comp ON competence.idCom = prof_comp.idCom LEFT JOIN profil_comp ON competence.idCom = profil_comp.idCom WHERE prof_comp.idCom IS NULL AND profil_comp.idCom IS NULL");
        else if(pro) request.exec("SELECT competence.nomCom FROM competence LEFT JOIN prof_comp ON competence.idCom = prof_comp.idCom WHERE prof_comp.idCom IS NULL");
        else if(pfl) request.exec("SELECT competence.nomCom FROM competence LEFT JOIN profil_comp ON competence.idCom = profil_comp.idCom WHERE profil_comp.idCom IS NULL");
        else request.exec("SELECT nomCom FROM competence");
        while(request.next()) listeCom << request.value(0).toString();
        if(!listeCom.isEmpty())
        {
            output << "CPT:" << endl;
            output << "\t" << listeCom.size() << endl;
            for(const QString& com : listeCom)
            {
                output << "\t" << com << endl;
            }
        }
        output << endl;
    }

    file.close();
    return true;
}

bool Database::removeComFromProfil(const QString& profil, const QString& competence)
{
    QStringList listProfils = listerProfils();
    if(listProfils.contains(profil))
    {
        int idPro = addProfil(profil);
        QStringList listComPro = listerComProfil(profil);
        if(listComPro.contains(competence))
        {
            int idCom = addCompetence(competence);
            QSqlQuery request;
            request.prepare("DELETE FROM profil_comp WHERE idPro = :idP AND idCom = :idC");
            request.bindValue(":idP", idPro);
            request.bindValue(":idC", idCom);
            return request.exec();
         }
         else return true;
    }
    else return true;
}

bool Database::removeComFromProf(const QString& nom, const QString& prenom, const QString& competence)
{
    QStringList listNProf, listPProfs;
    listerProfessionnels(listNProf, listPProfs);
    bool contain = false;
    int n = listNProf.size();
    for(int i=0;i<n;i++)
    {
        if(listNProf[i] == nom  && listPProfs[i] == prenom)
        {
            contain = true;
            break;
        }
    }
    if(contain)
    {
        int idProf = addProfessionnel(nom, prenom);
        QStringList listComProf = listerComProf(nom, prenom);
        if(listComProf.contains(competence))
        {
            int idCom = addCompetence(competence);
            QSqlQuery request;
            request.prepare("DELETE FROM prof_comp WHERE idProf = :idPf AND idCom = :idC");
            request.bindValue(":idPf", idProf);
            request.bindValue(":idC", idCom);
            return request.exec();
         }
         else return true;
    }
    else return true;
}

bool Database::removeProfil(const QString& profil)
{
    QStringList listProfils = listerProfils();
    if(listProfils.contains(profil))
    {
        int idPro = addProfil(profil);
        QSqlQuery request;
        request.prepare("DELETE FROM profil_comp WHERE idPro = :idpro");
        request.bindValue(":idpro", idPro);
        if(request.exec())
        {
            request.prepare("DELETE FROM profil WHERE nomPro = :pro");
            request.bindValue(":pro", profil);
            return request.exec();
        }
        return false;
    }
    else return true;
}

bool Database::removeProfessionnel(const QString& nom, const QString& prenom)
{
    QStringList listNProf, listPProfs;
    listerProfessionnels(listNProf, listPProfs);
    bool contain = false;
    int n = listNProf.size();
    for(int i=0;i<n;i++)
    {
        if(listNProf[i] == nom  && listPProfs[i] == prenom)
        {
            contain = true;
            break;
        }
    }
    if(contain)
    {
        int idProf = addProfessionnel(nom, prenom);
        QSqlQuery request;
        request.prepare("DELETE FROM prof_comp WHERE idProf = :idprof");
        request.bindValue(":idprof", idProf);
        if(request.exec())
        {
            request.prepare("DELETE FROM professionnel WHERE idProf = :idprof");
            request.bindValue(":idprof", idProf);
            return request.exec();
        }
        return false;
    }
    else return true;
}

bool Database::removeCompetence(const QString& competence)
{
    QStringList listCom = listerCompetences();
    if(listCom.contains(competence))
    {
        int idCom = addCompetence(competence);
        QSqlQuery request;
        request.prepare("DELETE FROM prof_comp WHERE idCom = :idcom");
        request.bindValue(":idcom", idCom);
        if(request.exec())
        {
            request.prepare("DELETE FROM profil_comp WHERE idCom = :idcom");
            request.bindValue(":idcom", idCom);
            if(request.exec())
            {
                request.prepare("DELETE FROM competence WHERE idCom = :idcom");
                request.bindValue(":idcom", idCom);
                return request.exec();
            }
            else return false;
        }
        else return false;
    }
    else return true;
}

bool Database::renameCompetence(const QString& oldCom, const QString& newCom)
{
    QSqlQuery request;
    request.prepare("UPDATE competence SET nomCom = :newcom WHERE nomCom = :oldcom");
    request.bindValue(":newcom", newCom);
    request.bindValue(":oldcom", oldCom);
    return request.exec();
}

bool Database::renameProfil(const QString& oldPro, const QString& newPro)
{
    QSqlQuery request;
    request.prepare("UPDATE profil SET nomPro = :newpro WHERE nomPro = :oldpro");
    request.bindValue(":newpro", newPro);
    request.bindValue(":oldpro", oldPro);
    return request.exec();
}

bool Database::renameProfessionnel(const QString& oldNom, const QString& oldPrenom, const QString& newNom, const QString& newPrenom)
{
    QSqlQuery request;
    request.prepare("UPDATE professionnel SET nomProf = :newnom, prenomProf = :newprenom WHERE nomProf = :oldnom AND prenomProf = :oldprenom");
    request.bindValue(":newnom", newNom);
    request.bindValue(":newprenom", newPrenom);
    request.bindValue(":oldnom", oldNom);
    request.bindValue(":oldprenom", oldPrenom);
    return request.exec();
}

QStringList Database::listerCompetences()
{
    QSqlQuery request;
    QStringList listeCom;
    request.prepare("SELECT nomCom FROM competence");
    request.exec();
    while(request.next())
    {
        listeCom << request.value(0).toString();
    }
    return listeCom;
}

QStringList Database::listerProfils()
{
    QSqlQuery request;
    QStringList listePro;
    request.prepare("SELECT nomPro FROM profil");
    request.exec();
    while(request.next())
    {
        listePro << request.value(0).toString();
    }
    return listePro;
}

void Database::listerProfessionnels(QStringList& listeNProf, QStringList& listePProf, bool sorted)
{
    QSqlQuery request;
    if(sorted) request.prepare("SELECT nomProf, prenomProf FROM professionnel ORDER BY nomProf ASC, prenomProf ASC");
    else request.prepare("SELECT nomProf, prenomProf FROM professionnel");
    request.exec();
    while(request.next())
    {
        listeNProf << request.value(0).toString();
        listePProf << request.value(1).toString();
    }
}

QStringList Database::listerComProf(const QString& nom, const QString& prenom)
{
    QStringList listeCom;
    QSqlQuery request;
    request.prepare("SELECT competence.nomCom FROM competence INNER JOIN prof_comp ON competence.idCom = prof_comp.idCom INNER JOIN professionnel ON professionnel.idProf = prof_comp.idProf WHERE professionnel.nomProf = :nom AND professionnel.prenomProf = :pre");
    request.bindValue(":nom", nom);
    request.bindValue(":pre", prenom);
    request.exec();
    while(request.next())
    {
        listeCom << request.value(0).toString();
    }
    return listeCom;
}

QStringList Database::listerComProfil(const QString& nom)
{
    QStringList listeCom;
    QSqlQuery request;
    request.prepare("SELECT competence.nomCom FROM competence INNER JOIN profil_comp ON competence.idCom = profil_comp.idCom INNER JOIN profil ON profil.idPro = profil_comp.idPro WHERE profil.nomPro = :nom");
    request.bindValue(":nom", nom);
    request.exec();
    while(request.next())
    {
        listeCom << request.value(0).toString();
    }
    return listeCom;
}

QStringList Database::listerProfilsProf(const QString& nom, const QString& prenom)
{
    QStringList listComProf, listProfilsProf, listProfil;
    listComProf = listerComProf(nom, prenom);
    listProfil = listerProfils();
    for(const QString& profil : listProfil)
        if(contains(listComProf, listerComProfil(profil)))
            listProfilsProf << profil;
    return listProfilsProf;
}

void Database::listerProfsProfil(const QString& nom, QStringList& listNProfsProfil, QStringList& listPProfsProfil)
{
    QStringList listComProfil, listNProfs, listPProfs;
    listComProfil = listerComProfil(nom);
    listerProfessionnels(listNProfs, listPProfs);
    int n = listNProfs.size();
    for(int i=0;i<n;i++)
        if(contains(listerComProf(listNProfs[i], listPProfs[i]), listComProfil))
        {
            listNProfsProfil << listNProfs[i];
            listPProfsProfil << listPProfs[i];
        }
}

void Database::strip(QString& mot)
{
    while(mot.startsWith(" ") || mot.startsWith("\n") || mot.startsWith("\t")) mot.remove(0, 1);
    while(mot.endsWith(" ") || mot.endsWith("\n") || mot.endsWith("\t")) mot.remove(mot.size()-1, 1);
    capitalize(mot);
}

void Database::capitalize(QString &mot)
{
    for(int i=0;i<mot.size();i++)
    {
        if(i==0) mot[i] = mot[i].toUpper();
        if(mot[i] == " ") mot[i+1] = mot[i+1].toUpper();
    }
}

bool Database::contains(const QStringList& big, const QStringList& small)
{
    for(const QString& str : small)
        if(!big.contains(str))
            return false;
    return true;
}
