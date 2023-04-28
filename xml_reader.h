#ifndef XML_READER_H
#define XML_READER_H
#include <QtCore>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include "common.h"
//XML文件读取器类
//公有继承于QXmlStreamReader
class CxmlReader : public QXmlStreamReader
{
    //Q_OBJECT
public:
    CxmlReader();
    bool read(QIODevice *device);   
    int getReadNum();//---获取已经读取的节点个数
    int getNmaeNum();//---获取父节点总个数
    void countNameType(const S_XML_NODE &father);//---统计有多少种Name
    QSet<QString> setNameType;//---存放几个Name类型以及个数
    QMap<QString,QVector<S_XML_NODE>> xml;//---存放所有的ID与对应的子节点集合
    QMap<QString,QVector<QString>> mapId;//---存放类目名称与对应的ID列表
private:
    QString readNode();//往下读取各个element
    int m_nodeNum;//节点个数
};

#endif // XML_READER_H
