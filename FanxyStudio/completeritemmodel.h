#ifndef COMPLETERITEMMODEL_H
#define COMPLETERITEMMODEL_H

#include <QAbstractListModel>
#include <QString>

#include "syntaxtemplateloader.h"

class CompleterItemModel : public QAbstractListModel
{
    Q_OBJECT

public : enum Roles
{
    NameRole = Qt::UserRole,
    InfoRole
};

public:
    CompleterItemModel(QObject *parent = 0);

    void        addItem(const SyntaxTemplateLoader::CompletionData &completionData);
    void        addItems(const SyntaxTemplateLoader::CompletionDataList &completionDataList);
    void        clear();

    int         rowCount(const QModelIndex &parent = QModelIndex()) const;
    int         columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant    data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    SyntaxTemplateLoader *m_syntaxTemplateLoader;
    SyntaxTemplateLoader::CompletionDataList m_completionDataList;
};


#endif // COMPLETERITEMMODEL_H
