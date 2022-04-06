#include "shape_list_widget.h"

#include <QFile>
#include <QStringList>
#include <algorithm>

ShapeListWidget::ShapeListWidget(QWidget* parent)
		: QDockWidget(parent),
			model_(new TreeModel(this)),
			view_(new QTreeView(this)) {
	// example with checkboxes
	// https://stackoverflow.com/questions/14158191/qt-qtreeview-and-custom-model-with-checkbox-columns
	view_->setWindowTitle(QObject::tr("Simple Tree Model"));
	setWidget(view_);

	// Test();
}

void ShapeListWidget::Test() {
	// model_->TestFillWithTxtFile(
	// 		"C:\\Users\\winroot\\Desktop\\overheat\\local_docs\\default.txt");
}

// TreeItem

TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parent)
		: item_data_(data), parent_item_(parent) {}

TreeItem* TreeItem::child(int row) {
	if (row < 0 || row >= child_items_.size())
		return nullptr;
	return child_items_.at(row);
}

QVariant TreeItem::data(int column) const {
	if (column < 0 || column >= item_data_.size())
		return {};
	return item_data_.at(column);
}

int TreeItem::row() const {
	if (parent_item_)
		return static_cast<int>(parent_item_->child_items_.indexOf(this));

	return 0;
}

// Tree Model

TreeModel::TreeModel(QObject* parent) : QAbstractItemModel(parent) {
	rootItem = new TreeItem({tr("Name"), tr("Visibility"), tr("Highlight")});
}

// NOLINTNEXTLINE(google-default-arguments)
int TreeModel::columnCount(const QModelIndex& parent) const {
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->column_count();
	return rootItem->column_count();
}

QVariant TreeModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid())
		return {};
	if (role != Qt::DisplayRole)
		return {};
	auto* item = static_cast<TreeItem*>(index.internalPointer());
	return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const {
	return !index.isValid() ? QAbstractItemModel::flags(index) : Qt::NoItemFlags;
}

// NOLINTNEXTLINE(google-default-arguments)
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);
	return {};
}

// NOLINTNEXTLINE(google-default-arguments)
QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex& parent) const {
	if (!hasIndex(row, column, parent))
		return {};
	TreeItem* parentItem = nullptr;
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());
	TreeItem* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return {};
}

QModelIndex TreeModel::parent(const QModelIndex& index) const {
	if (!index.isValid())
		return {};
	auto* childItem      = static_cast<TreeItem*>(index.internalPointer());
	TreeItem* parentItem = childItem->parentItem();
	if (parentItem == rootItem)
		return {};
	return createIndex(parentItem->row(), 0, parentItem);
}

// NOLINTNEXTLINE(google-default-arguments)
int TreeModel::rowCount(const QModelIndex& parent) const {
	if (parent.column() > 0)
		return 0;
	TreeItem* parentItem = nullptr;
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->child_count();
}

void TreeModel::TestFillWithTxtFile(const QString& file_path) {
	QFile file(file_path);
	file.open(QIODevice::ReadOnly);
	QString raw_data = file.readAll();
	QStringList lines = raw_data.split('\n');
	file.close();

	QVector<TreeItem*> parents;
	QVector<int> indentations;
	parents << rootItem;
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

				if (parents.last()->child_count() > 0) {
					parents << parents.last()->child(parents.last()->child_count() - 1);
					indentations << position;
				}
			} else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}
			// Append a new item to the current parent's list of children.
			parents.last()->append_child(new TreeItem(columnData, parents.last()));
		}
		++number;
	}
}
