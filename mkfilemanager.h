#ifndef MKFILEMANAGER_H
#define MKFILEMANAGER_H

#include <QObject>
#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
// Library needed for processing XML documents
#include <QtXml>
// Library needed for processing files
#include <QFile>
//#include "mkglobaldatastructure.h"
class mkFileManager : public QObject
{
    Q_OBJECT
public:
    explicit mkFileManager(QObject *parent = nullptr);
    ~mkFileManager();
public:
    //std::vector<std::tuple<std::string,std::string,double, double, double, double, double, double>> armConfigAng;
public:
    bool readEEXML(char * fileName);
    bool readTargetListXML(char * fileName);
    bool saveTargetListXML(char *fileName);
    bool parseArmConfigListsXML(QDomNodeList &armConfigLists);

signals:

public slots:
};

#endif // MKFILEMANAGER_H
