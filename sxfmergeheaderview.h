#ifndef SXFMERGEHEADERVIEW_H
#define SXFMERGEHEADERVIEW_H

#include <QHeaderView>
#include <QPainter>
#include <QMap>

// --- New Includes ---
#include <QMouseEvent>

class SxfMergeHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    SxfMergeHeaderView(Qt::Orientation orientation, QWidget* parent = nullptr);

    QSize sectionSizeFromContents(int logicalIndex) const override;
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;

    // Public interface to allow external recalculation of groups
    void calculateGroups();

signals:
    // --- New Signal ---
    // Emitted when a header section is clicked, passing its logical index
    void columnSelected(int logicalIndex);

protected:
    // --- New Event Override ---
    void mousePressEvent(QMouseEvent* event) override;

private:
    // Grouping info: Key is group name ("ACTION" or "CELL"), Value is list of logical column indices.
    QMap<QString, QList<int>> m_groups;

private slots:
    void handleSectionResized(int logicalIndex, int oldSize, int newSize);
};

#endif // SXFMERGEHEADERVIEW_H