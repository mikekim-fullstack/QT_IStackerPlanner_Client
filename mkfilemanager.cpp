#include "mkfilemanager.h"
#include <QDateTime>
#include <QDate>
#include <iostream>
#include <string>
#include <tuple>

#include "mainwindow.h"
#include "renderrobotarm.h"
#include "mkGlobalData.h"
extern MainWindow *g_MainWindow;

mkFileManager::mkFileManager(QObject *parent) : QObject(parent)
{

}

mkFileManager::~mkFileManager()
{

}

bool mkFileManager::readEEXML(char *fileName)
{
    QString curDir = qApp->applicationDirPath();
    QDomDocument xmlEELIST;
    // ___________________________
    // ::: Read data from file :::
    // Load xml file as raw data
    QFile f(curDir+fileName);
    qDebug()<<curDir+fileName;
    if (!f.open(QIODevice::ReadOnly ))
    {
        // Error while loading file
        qDebug() << "Error while loading file::readEEXML" ;
        return false;
    }

    // Set data into the QDomDocument before processing
    xmlEELIST.setContent(&f);
    f.close();
    return true;
}

bool mkFileManager::readTargetListXML(char *fileName)
{
    QString curDir = qApp->applicationDirPath();
    QDomDocument xmlTargetList;
    // ___________________________
    // ::: Read data from file :::
    // Load xml file as raw data
   // QFile f(curDir+fileName);
    //qDebug()<<curDir+fileName;
    QFile f(fileName);
    qDebug()<<fileName;
    if (!f.open(QIODevice::ReadOnly ))
    {
        // Error while loading file
        qDebug() << "Error while loading file::readEEXML" ;
        return false;
    }

    // Set data into the QDomDocument before processing
    xmlTargetList.setContent(&f);
    f.close();

    // _________________________
    // ::: Read  :::
    QDomElement root = xmlTargetList.documentElement();
    QDomElement targetCollection = root.firstChild().toElement();
    qDebug()<<root.tagName();
    std::string typeMotion[2]={""};
    double posTarget[4];
    if(root.tagName()=="TARGET_COLLECTIONS")
    {

        while(!targetCollection.isNull())
        {
            bool bType=false, bPos=false;
            QString attriName = targetCollection.attribute("NAME","empty");
            QString attriDesc = targetCollection.attribute("DESC","empty");
            qDebug() << "Tag:" <<targetCollection.tagName()<< "NAME-->"<<attriName<<"DESC-->"<<attriDesc;
            // +++ Read elements from child of component... +++
            QDomElement target=targetCollection.firstChild().toElement();
            while(!target.isNull()) {
                if (targetCollection.tagName() == "TARGET" && target.tagName()=="TYPE")
                {
                    QString type=target.firstChild().toText().data();
                    type.replace(QString(","), QString(" "));
                    QStringList typeLists = type.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                    for(int i=0; i<2; i++)
                    {
                        typeMotion[i]=typeLists.at(i).toStdString();
                    }
                    bType=true;
                    qDebug()<<"\tType: "<< typeMotion[0].c_str()<<typeMotion[1].c_str();
                }
                else if (targetCollection.tagName() == "TARGET" && target.tagName()=="POSITION")
                {

                    QString pos=target.firstChild().toText().data();
                    pos.replace(QString(","), QString(" "));
                    QStringList posLists = pos.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                    for(int i=0; i<4; i++)
                    {
                        posTarget[i]=posLists.at(i).toDouble();
                    }
                    bPos=true;
                    qDebug()<<"\tPosition: "<< posTarget[0]<<posTarget[1]<<posTarget[2]<<posTarget[3];
                }
                target = target.nextSibling().toElement();
            }
            if(bType && bPos)
            {
                 g_MainWindow->renderRobotFront->addTargetFromeFile(attriName.toStdString(), attriDesc.toStdString(), typeMotion, posTarget);
            }
            qDebug()<<"------------------------- Sibling End---------------------\n";
            QDomElement Child=targetCollection.firstChild().toElement();
            targetCollection = targetCollection.nextSibling().toElement();

        }
    }
    ///////////////////////////////////////////

    return true;
}

bool mkFileManager::saveTargetListXML(char *fileName)
{
    QString curDir = qApp->applicationDirPath();
    // ___________________________
    // ::: Read data from file :::
    // Load xml file as raw data
   // double jntAng[6];
    char buff[256];
    // Create a document to write XML
        QDomDocument document;

        // Making the root element
        QDomElement root = document.createElement("TARGET_COLLECTIONS");
        document.appendChild(root);
        for(MapObjActor * objTarget: g_MainWindow->renderRobotFront->objTarget) {
            QDomElement target = document.createElement("TARGET");
            target.setAttribute("NAME", objTarget->param.name);
            target.setAttribute("DESC", objTarget->param.desc);
            root.appendChild(target);

                QDomElement type = document.createElement("TYPE");
                sprintf(buff, "%s, %s", objTarget->param.getTargetType().c_str(), objTarget->param.getMoveType().c_str());
                qDebug()<<buff;
                QDomText typeValue = document.createTextNode(buff);
                type.appendChild(typeValue);

                QDomElement pos = document.createElement("POSITION");
               // pos.setAttribute("DEST_TYPE", objTarget->)
                sprintf(buff,"%3.4f, %3.4f, %3.4f, %3.4f",
                        objTarget->param.posTargetEE[0], objTarget->param.posTargetEE[1], objTarget->param.posTargetEE[2], objTarget->param.posTargetEE[3]);
                QDomText value = document.createTextNode(buff);
                pos.appendChild(value);

                target.appendChild(type);
                target.appendChild(pos);
        }
        // Writing to a file
           // QFile file(curDir+fileName);
            QFile file(fileName);

            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "Open the file for writing failed";
            }
            else
            {
                QTextStream stream(&file);
                stream << document.toString();
                file.close();
                qDebug() << "Writing file is done"<<fileName<<QDateTime::currentDateTime().toString(Qt::ISODate).simplified().replace( " ", "" );
//                qDebug() << "Writing is done"<<curDir+fileName;
            }

            ////////////////////////////////////////////
            // Write Backup file with current time stamp
            QFile fileBK(QString(fileName)+"."+QDateTime::currentDateTime().toString(Qt::ISODate).simplified().replace( ":", "-" ));

            if(!fileBK.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "Open the file for writing failed";
            }
            else
            {
                QTextStream stream(&fileBK);
                stream << document.toString();
                fileBK.close();
            }
                 return true;
}

bool mkFileManager::parseArmConfigListsXML(QDomNodeList &armConfigLists)
{
    /*
    for(int i = 0; i < armConfigLists.size(); ++i)
    {
        QDomElement armConfigElement=armConfigLists.at(i).toElement();
        QDomElement angleElement=armConfigElement.firstChild().toElement();
        // Loop while there is a child
        while(!angleElement.isNull())
        {
            // Check if the child tag name is componentJOINT
            if (angleElement.tagName()=="ANGLE")
            {
                // Read Name and value
                QString side=angleElement.attribute("SIDE","");
                QString angle=angleElement.firstChild().toText().data();
                angle.replace(QString(","), QString(" "));
                QStringList ang6 = angle.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                if(ang6.size()!=6) {
                    qDebug()<<"file corrupted: joint angle should be six!!"<<ang6.size();
                    return false;
                }
                qDebug()<<side<<angle<<ang6.size();
                gSharedData.armConfigAngOriginal.emplace_back(armConfigElement.attribute("NAME","empty").toUtf8().constData(), side.toUtf8().constData(), ang6.at(0).toDouble(), ang6.at(1).toDouble(),  ang6.at(2).toDouble(), ang6.at(3).toDouble(), ang6.at(4).toDouble(), ang6.at(5).toDouble());
                gSharedData.armConfigAng.emplace_back(armConfigElement.attribute("NAME","empty").toUtf8().constData(), side.toUtf8().constData(), ang6.at(0).toDouble(), ang6.at(1).toDouble(),  ang6.at(2).toDouble(), ang6.at(3).toDouble(), ang6.at(4).toDouble(), ang6.at(5).toDouble());
            }
            // Next componentANGLETAG
            angleElement = angleElement.nextSibling().toElement();
        }// --- while(joint)
    }
    */
    return true;
}
