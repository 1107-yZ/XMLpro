//====================================================================
// 文件名: xml_reader.cpp
//
// 文件描述:
// ------------------------------------------------------------------
// QXmlStreamReader类解析标准XML文件
// ------------------------------------------------------------------
//
// 时间: 2017.8
// 编程: 吴建栋
// ------------------------------------------------------------------
// 修改说明(请按格式说明)...
//====================================================================
#include <QDebug>
#include <qalgorithms.h>
#include <QSet>
#include "xml_reader.h"

CxmlReader::CxmlReader()
{
    m_nodeNum = 0;
}

bool CxmlReader::read(QIODevice *device)
{
    setDevice(device);
    readNext();
    if(hasError())
    {
        qDebug()<<"读取到坏文件";
        return false;
    }
    if("ok" != readNode())
    {
        return false;
    }
    return true;
}

int CxmlReader::getNmaeNum()
{
    return setNameType.count();
}

void CxmlReader::countNameType(const S_XML_NODE &father)
{
    setNameType.insert(father.Name);
}

//---读取节点并且保持
QString CxmlReader::readNode()
{
    QString lastname = "RDF";
    QVector<QString> VecID;
    while(!atEnd())
    {
        if(isStartElement() && !attributes().isEmpty())
        {
            if("ID" == attributes().begin()->name())//---父节点
            {
                if(name() != lastname)
                {
                    //---保存Vector到Map
                    mapId.insert(lastname,VecID);
                    //---清理Vector
                    VecID.clear();

                }
                VecID.append(attributes().begin()->value().toString());
                //---更新lastname
                lastname = name().toString();
                //保存父亲节点的属性
                S_XML_NODE father;
                father.Name = name().toString();
                father.value = attributes().begin()->value().toString();

                readNext();//---读掉换行
                if(hasError())
                {
                    qDebug()<<"读取到坏文件";
                    return errorString();
                }
                readNext();//---读到子节点
                if(hasError())
                {
                    qDebug()<<"读取到坏文件";
                    return errorString();
                }

                S_XML_NODE child;
                child.same = false;

                while (name().toString() != father.Name)//---子节点
                {
                    while(!isEndElement())
                    {
                        if(isStartElement())//---读到<
                        {
                            child.Name = name().toString();//---保存name
                            if(!attributes().isEmpty())//---保存属性
                            {                               
                                child.value = attributes().begin()->value().toString();
                            }
                            readNext();
                            if(hasError())
                            {
                                qDebug()<<"读取到坏文件";
                                return errorString();
                            }
                            continue;
                        }
                        if(isCharacters())//读到文本
                        {
                             child.text = text().toString();
                             readNext();
                             continue;
                        }
                    }
                    //---如果读到了/>，就保存整条记录，一条子节点读完了
                    father.childVec.append(child);                   
                    child.Name.clear();
                    child.text.clear();
                    child.value.clear();
                    readNext();//---读掉换行
                    if(hasError())
                    {
                        qDebug()<<"读取到坏文件";
                        return errorString();
                    }
                    readNext();
                    if(hasError())
                    {
                        qDebug()<<"读取到坏文件";
                        return errorString();
                    }
                }//---父节点读完了
                //---排序操作
                xml.insert(father.value,father.childVec);
                countNameType(father);//统计节点类别
            }
        }
        readNext();
        //---判断错误跳出
        if(hasError())
        {
            return errorString();
        }
    }
    if(atEnd())
    {
        //---保存Vector到Map
        mapId.insert(lastname,VecID);
        //---清理Vector
        VecID.clear();
        mapId.erase(mapId.find("RDF"));
    }


    return QString("ok");

}
