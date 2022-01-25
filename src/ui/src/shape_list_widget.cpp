#include "shape_list_widget.h"

#include "ui_shape_list_widget.h"


#include <QFile>
#include <QTreeView>

ShapeListWidget::ShapeListWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::ShapeListWidget) {
	ui_->setupUi(this);


	// example with checkboxes https://stackoverflow.com/questions/14158191/qt-qtreeview-and-custom-model-with-checkbox-columns
	QFile file("C:\\Users\\winroot\\Desktop\\overheat\\local_docs\\default.txt");
	file.open(QIODevice::ReadOnly);
	static TreeModel* model  = new TreeModel(file.readAll(), this);
	file.close();
	static QTreeView* view = new QTreeView(this);
	view->setModel(model);
	view->setWindowTitle(QObject::tr("Simple Tree Model"));
	setWidget(view);
}

ShapeListWidget::~ShapeListWidget() { delete ui_; }


// ONLY FOR EXAMPLE

#include <QStringList>

//! [0]
TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parent)
		: m_itemData(data), m_parentItem(parent) {}
//! [0]

//! [1]
TreeItem::~TreeItem() { qDeleteAll(m_childItems); }
//! [1]

//! [2]
void TreeItem::appendChild(TreeItem* item) { m_childItems.append(item); }
//! [2]

//! [3]
TreeItem* TreeItem::child(int row) {
	if (row < 0 || row >= m_childItems.size())
		return nullptr;
	return m_childItems.at(row);
}
//! [3]

//! [4]
int TreeItem::childCount() const { return m_childItems.count(); }
//! [4]

//! [5]
int TreeItem::columnCount() const { return m_itemData.count(); }
//! [5]

//! [6]
QVariant TreeItem::data(int column) const {
	if (column < 0 || column >= m_itemData.size())
		return QVariant();
	return m_itemData.at(column);
}
//! [6]

//! [7]
TreeItem* TreeItem::parentItem() { return m_parentItem; }
//! [7]

//! [8]
int TreeItem::row() const {
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}
//! [8]
//! 
//! 
//! 

//! [0]
TreeModel::TreeModel(const QString& data, QObject* parent)
		: QAbstractItemModel(parent) {
	rootItem = new TreeItem({tr("Title"), tr("Summary")});
	setupModelData(data.split('\n'), rootItem);
}
//! [0]

//! [1]
TreeModel::~TreeModel() { delete rootItem; }
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex& parent) const {
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	return rootItem->columnCount();
}
//! [2]

//! [3]
QVariant TreeModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column());
}
//! [3]

//! [4]
Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const {
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}
//! [5]

//! [6]
QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex& parent) const {
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem* parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}
//! [6]

//! [7]
QModelIndex TreeModel::parent(const QModelIndex& index) const {
	if (!index.isValid())
		return QModelIndex();

	TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem* parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int TreeModel::rowCount(const QModelIndex& parent) const {
	TreeItem* parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}
//! [8]

void TreeModel::setupModelData(const QStringList& lines, TreeItem* parent) {
	QVector<TreeItem*> parents;
	QVector<int> indentations;
	parents << parent;
	indentations << 0;

	int number = 0;

	while (number < lines.count()) {
		int position = 0;
		while (position < lines[number].length()) {
			if (lines[number].at(position) != ' ')
				break;
			position++;
		}

		const QString lineData = lines[number].mid(position).trimmed();

		if (!lineData.isEmpty()) {
			// Read the column data from the rest of the line.
			const QStringList columnStrings =
					lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
			QVector<QVariant> columnData;
			columnData.reserve(columnStrings.count());
			for (const QString& columnString : columnStrings)
				columnData << columnString;

			if (position > indentations.last()) {
				// The last child of the current parent is now the new parent
				// unless the current parent has no children.

				if (parents.last()->childCount() > 0) {
					parents << parents.last()->child(parents.last()->childCount() - 1);
					indentations << position;
				}
			} else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}

			// Append a new item to the current parent's list of children.
			parents.last()->appendChild(new TreeItem(columnData, parents.last()));
		}
		++number;
	}
}
