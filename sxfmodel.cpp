#include "sxfmodel.h"
#include <QMap>
#include <QStringList>

namespace {
    // 映射表用于显示和输入
    const QMap<quint16, QString> cellTypeToStrMap = {
        {0x0000, ""},
        {0x0001, "#"},
        {0x0002, "○"},
        {0x0004, "●"},
        {0x0008, "×"}
    };

    const QMap<QString, quint16> strToCellTypeMap = {
        {"", 0x0000},
        {"#", 0x0001},
        {"○", 0x0002},
        {"●", 0x0004},
        {"×", 0x0008}
    };
} // end anonymous namespace

SxfModel::SxfModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int SxfModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_sxfData.property.maxFrames;
}

int SxfModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    int actionCols = m_sxfData.actionSheet.columns.length();
    int cellCols = m_sxfData.cellSheet.columns.length();

    if (actionCols == 0 && cellCols == 0)
        return 0;

    return actionCols + cellCols + 1; // +1 for "Frame" column
}

QVariant SxfModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row(); // This is the frame number (0-based)
    int col = index.column();

    if (role == Qt::DisplayRole) {
        if (col == 0) {
            // First column is frame number (1-based)
            return QString::number(row + 1);
        }

        // Find the correct column
        const SxfColumn* column = nullptr;
        int actionSheetColCount = m_sxfData.actionSheet.columns.length();
        int cellIdx = col - 1; // 0-based index of data columns

        int columnArea = getColumnArea(col); // 0 = ACTION, 1 = CELL

        if (columnArea == 0) { // ACTION
            column = &(m_sxfData.actionSheet.columns[cellIdx]);
        }
        else if (columnArea == 1) { // CELL
            cellIdx -= actionSheetColCount;
            column = &(m_sxfData.cellSheet.columns[cellIdx]);
        }

        if (column) {
            // --- 修复点 (V6) ---
            // 这是一个 DENSE 列表。'row' 是 'cells' 列表的索引。
            if (row < column->cells.length()) {
                const SxfCell& cell = column->cells[row];
                quint16 mark = cell.mark;
                quint32 frameId = cell.frameIndex; // "原画编号"

                // 真实的空单元格
                if (mark == 0 && frameId == 0) {
                    return "";
                }

                QString numberStr = QString::number(frameId);
                QString symbolStr = cellTypeToStrMap.value(mark, ""); // 查找符号

                if (symbolStr.isEmpty()) {
                    // 找不到对应符号 (或 mark=0 但 frameId!=0)，只显示数字
                    return numberStr;
                }
                else {
                    // 找到了符号，组合显示
                    return symbolStr + " " + numberStr;
                }
            }
            // --- 结束修复 ---
        }
        // 'row' 越界 (理论上不应发生，如果 loadData 正确填充)
        return "";
    }

    return QVariant();
}

bool SxfModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    int row = index.row(); // This is the frame (0-based)
    int col = index.column();

    if (col == 0) {
        // First column (frame number) is read-only
        return false;
    }

    // Find the correct column
    SxfColumn* column = nullptr;
    int actionSheetColCount = m_sxfData.actionSheet.columns.length();
    int cellIdx = col - 1; // 0-based index of data columns

    int columnArea = getColumnArea(col); // 0 = ACTION, 1 = CELL

    if (columnArea == 0) { // ACTION
        column = &(m_sxfData.actionSheet.columns[cellIdx]);
    }
    else if (columnArea == 1) { // CELL
        cellIdx -= actionSheetColCount;
        column = &(m_sxfData.cellSheet.columns[cellIdx]);
    }

    if (!column) {
        return false; // Column out of bounds
    }

    // --- 修复点 (V6) ---

    // 1. 如果需要，扩展 'cells' 列表以适应新行
    while (row >= column->cells.length()) {
        column->cells.append(SxfCell());
    }

    // 2. 获取单元格的引用
    SxfCell& cell = column->cells[row];

    // 3. 解析输入值
    QString strValue = value.toString().trimmed();
    quint16 newType = cell.mark; // 默认保留旧 type
    quint32 newFrameId = cell.frameIndex; // 默认保留旧 frameId

    if (strValue.isEmpty()) {
        newType = 0;
        newFrameId = 0;
    }
    else {
        QStringList parts = strValue.split(' ', Qt::SkipEmptyParts);
        bool symbolFound = false;
        bool numberFound = false;
        bool part1_is_sym = false;
        bool part1_is_num = false;

        if (parts.length() == 1)
        {
            // --- 输入是 "5" (数字) ---
            bool ok;
            quint32 num = parts[0].toUInt(&ok);
            if (ok) {
                newFrameId = num;
                numberFound = true;
                // 自动设置/保留 type
                if (cell.mark == 0) { // 如果之前是空的
                    newType = 0x0002; // 默认给 "○"
                } // 否则保留旧符号
            }
            // --- 输入是 "●" (符号) ---
            else if (strToCellTypeMap.contains(parts[0])) {
                newType = strToCellTypeMap.value(parts[0]);
                symbolFound = true;
                // 自动设置/保留 frameId
                if (cell.frameIndex == 0) { // 如果之前是空的
                    newFrameId = 1; // 默认给 1
                } // 否则保留旧 frameId
            }
            else {
                return false; // 输入无效
            }
        }
        else if (parts.length() == 2)
        {
            // --- 输入是 "● 5" (符号 数字) ---
            if (strToCellTypeMap.contains(parts[0])) {
                bool ok;
                quint32 num = parts[1].toUInt(&ok);
                if (ok) {
                    newType = strToCellTypeMap.value(parts[0]);
                    newFrameId = num;
                    symbolFound = true;
                    numberFound = true;
                }
            }
            // --- 输入是 "5 ●" (数字 符号) ---
            else if (strToCellTypeMap.contains(parts[1])) {
                bool ok;
                quint32 num = parts[0].toUInt(&ok);
                if (ok) {
                    newFrameId = num;
                    newType = strToCellTypeMap.value(parts[1]);
                    symbolFound = true;
                    numberFound = true;
                }
            }
            else {
                return false; // 输入无效
            }
        }
        else {
            return false; // 输入无效 (0 或 >2 parts)
        }
    }

    // 4. 应用更改
    cell.mark = newType;
    cell.frameIndex = newFrameId;

    // 5. 更新总帧数 (如果需要)
    if (row >= m_sxfData.property.maxFrames) {
        m_sxfData.property.maxFrames = row + 1;
        // 调用 loadData 以便填充所有其他列
        loadData(m_sxfData);
    }
    else {
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
    }
    // --- 结束修复 ---
    return true;
}

QVariant SxfModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0) {
            return "Frame";
        }

        // Find the correct column
        const SxfColumn* column = nullptr;
        int actionSheetColCount = m_sxfData.actionSheet.columns.length();
        int cellIdx = section - 1; // 0-based index of data columns

        int columnArea = getColumnArea(section); // 0 = ACTION, 1 = CELL

        if (columnArea == 0) { // ACTION
            column = &(m_sxfData.actionSheet.columns[cellIdx]);
        }
        else if (columnArea == 1) { // CELL
            cellIdx -= actionSheetColCount;
            column = &(m_sxfData.cellSheet.columns[cellIdx]);
        }

        if (column) {
            return column->name;
        }
    }
    else { // Qt::Vertical
        return QString::number(section + 1); // 1-based frame number
    }
    return QVariant();
}

Qt::ItemFlags SxfModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == 0) {
        // 帧编号列是只读的
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    // 其他所有单元格都是可编辑的
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void SxfModel::loadData(const SxfData& data)
{
    beginResetModel();
    m_sxfData = data;

    // --- 修复点 (V6) ---
    // 确保所有 'cells' 列表都填充到 'maxFrames'
    // 这样模型才是 DENSE 的，而不是 SPARSE 的
    quint32 maxFrames = m_sxfData.property.maxFrames;

    for (SxfColumn& col : m_sxfData.actionSheet.columns) {
        while (col.cells.length() < maxFrames) {
            col.cells.append(SxfCell()); // SxfCell() 默认为 {0, 0}
        }
    }
    for (SxfColumn& col : m_sxfData.cellSheet.columns) {
        while (col.cells.length() < maxFrames) {
            col.cells.append(SxfCell());
        }
    }
    // --- 结束修复 ---

    endResetModel();
}

SxfData SxfModel::getData() const
{
    return m_sxfData;
}

// ============== 改造部分：新增 getColumnArea 函数 ==============

/**
 * @brief 获取指定列的 area 类型
 * @param column 0 代表 "Frame" 列；1 及以上代表 SxfData::columns 中的列
 * @return 0 代表 ACTION，1 代表 CELL；如果 column = 0 (Frame) 或越界，返回 -1
 */
int SxfModel::getColumnArea(int column) const
{
    if (column == 0) {
        // "Frame" 列
        return -1;
    }

    int actionSheetColCount = m_sxfData.actionSheet.columns.length();
    int cellIdx = column - 1; // 0-based index of data columns

    if (cellIdx < actionSheetColCount) {
        return 0; // ACTION
    }

    cellIdx -= actionSheetColCount;
    if (cellIdx < m_sxfData.cellSheet.columns.length()) {
        return 1; // CELL
    }

    // 越界
    return -1;
}

// =========================================================