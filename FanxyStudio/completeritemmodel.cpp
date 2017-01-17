#include "completeritemmodel.h"

#include <QIcon>
#include <QFile>

CompleterItemModel::CompleterItemModel(QObject *parent) : QAbstractListModel(parent)
{
    m_syntaxTemplateLoader = SyntaxTemplateLoader::instance();
}

void CompleterItemModel::addItem(const SyntaxTemplateLoader::CompletionData &completionData)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_completionDataList.append(completionData);
    endInsertRows();
}

void CompleterItemModel::addItems(const SyntaxTemplateLoader::CompletionDataList &completionDataList)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_completionDataList.append(completionDataList);
    endInsertRows();
}

void CompleterItemModel::clear()
{
   m_completionDataList.clear();
}

int CompleterItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_completionDataList.count();
}

int CompleterItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant CompleterItemModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_completionDataList.count())
        return QVariant();

    const SyntaxTemplateLoader::CompletionData &completionData = m_completionDataList[index.row()];

    switch(role)
    {
        //case Qt::DisplayRole:
        //    return QIcon::addFile("");
        case Qt::DisplayRole:
            return QString(completionData.name + " " + completionData.info);
        case NameRole:
            return completionData.name;
        case InfoRole:
            return completionData.info;
    }

    return QVariant();
}
