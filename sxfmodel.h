#ifndef SXFMODEL_H
#define SXFMODEL_H

#include <QAbstractTableModel>
#include "sxfprocessor.h"

class SxfModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SxfModel(QObject* parent = nullptr);

    // QAbstractTableModel 接口
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // 数据操作
    void loadData(const SxfData& data);
    SxfData getData() const;

    int getColumnArea(int column) const;

private:
    SxfData m_sxfData;
};

#endif // SXFMODEL_H
