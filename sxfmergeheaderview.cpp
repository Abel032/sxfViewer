#include "sxfmergeheaderview.h"
#include "sxfmodel.h" 
#include <QStyleOption>
#include <QStyle>

SxfMergeHeaderView::SxfMergeHeaderView(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent)
{
    setSectionsClickable(true);
    setStretchLastSection(true);
    connect(this, &QHeaderView::sectionResized, this, &SxfMergeHeaderView::handleSectionResized);
}

// --- New Method Implementation ---

/**
 * @brief Handles mouse press events on the header.
 * Emits the columnSelected signal with the logical index of the clicked section.
 * @param event The mouse event.
 */
void SxfMergeHeaderView::mousePressEvent(QMouseEvent* event)
{
    // Get the logical index of the column at the click position
    int logicalIndex = logicalIndexAt(event->pos());

    // Emit the signal
    if (logicalIndex >= 0) {
        emit columnSelected(logicalIndex);
    }

    // Call the base class implementation to handle default behavior (like resizing)
    QHeaderView::mousePressEvent(event);
}

// --- (El resto de sxfmergeheaderview.cpp no cambia: calculateGroups, paintSection, etc.) ---

void SxfMergeHeaderView::calculateGroups()
{
    m_groups.clear();
    const SxfModel* model = qobject_cast<const SxfModel*>(this->model());
    if (!model) return;

    if (model->columnCount() > 0) {
        m_groups["Frame"].append(0);
    }

    for (int i = 1; i < model->columnCount(); ++i) {
        int area = model->getColumnArea(i);
        QString groupName;

        if (area == 0) {
            groupName = "ACTION";
        }
        else if (area == 1) {
            groupName = "CELL";
        }
        else {
            continue;
        }

        m_groups[groupName].append(i);
    }
}

QSize SxfMergeHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    QSize size = QHeaderView::sectionSizeFromContents(logicalIndex);
    size.setHeight(size.height() * 2);
    return size;
}

void SxfMergeHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    if (m_groups.isEmpty()) {
        const_cast<SxfMergeHeaderView*>(this)->calculateGroups();
    }

    QStyleOptionHeader opt;
    initStyleOption(&opt);
    opt.section = logicalIndex;

    if (logicalIndex == 0) {
        opt.rect = rect;
        opt.text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();
        opt.textAlignment = Qt::AlignCenter;
        style()->drawControl(QStyle::CE_HeaderSection, &opt, painter);
        style()->drawControl(QStyle::CE_HeaderLabel, &opt, painter);
    }
    else {
        QRect bottomRect = rect;
        bottomRect.setTop(rect.center().y());
        opt.rect = bottomRect;
        style()->drawControl(QStyle::CE_HeaderSection, &opt, painter);
        opt.text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();
        style()->drawControl(QStyle::CE_HeaderLabel, &opt, painter);

        QRect topRect = rect;
        topRect.setBottom(rect.center().y());
        QString currentGroupName;
        for (auto it = m_groups.constBegin(); it != m_groups.constEnd(); ++it) {
            if (it.value().contains(logicalIndex)) {
                currentGroupName = it.key();
                break;
            }
        }

        if (currentGroupName.isEmpty()) return;

        if (m_groups.value(currentGroupName).first() == logicalIndex) {
            int groupWidth = 0;
            const QList<int>& indices = m_groups.value(currentGroupName);
            for (int index : indices) {
                groupWidth += sectionSize(index);
            }

            opt.rect = topRect;
            opt.rect.setWidth(groupWidth);
            opt.text = currentGroupName;
            opt.textAlignment = Qt::AlignCenter;
            style()->drawControl(QStyle::CE_HeaderSection, &opt, painter);
            style()->drawControl(QStyle::CE_HeaderLabel, &opt, painter);
        }
    }
}

void SxfMergeHeaderView::handleSectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(logicalIndex);
    Q_UNUSED(oldSize);
    Q_UNUSED(newSize);
    this->viewport()->update();
}