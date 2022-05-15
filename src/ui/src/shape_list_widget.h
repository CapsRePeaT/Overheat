#pragma once

#include "common.h"

#include <QAbstractItemModel>
#include <QDockWidget>
#include <QModelIndex>
#include <QTreeView>
#include <QVariant>
#include <QVector>



// TODO: remake with smart_ptrs

class TreeModel;

// top level item should be a design
class ShapeListWidget : public QDockWidget {
	Q_OBJECT

 public:
	explicit ShapeListWidget(QWidget* parent = nullptr);
	void AddData(const InstanceList& data);
	void ClearData(const GlobalId& id);
	void ClearAll();
 private:
	void Test();
	TreeModel* model_ = nullptr;
	QTreeView* view_  = nullptr;
};

class TreeItem {
 public:
	explicit TreeItem(const QVector<QVariant>& data,
	                  TreeItem* parentItem = nullptr);
	~TreeItem() { qDeleteAll(child_items_); }
	void append_child(TreeItem* item) { child_items_.append(item); }
	TreeItem* child(int row);
	[[nodiscard]] int child_count() const { return child_items_.count(); }
	[[nodiscard]] int column_count() const { return item_data_.count(); }
	[[nodiscard]] QVariant data(int column) const;
	[[nodiscard]] int row() const;
	TreeItem* parentItem() { return parent_item_; }

 private:
	QVector<TreeItem*> child_items_;
	QVector<QVariant> item_data_;
	TreeItem* parent_item_;
};

class TreeModel : public QAbstractItemModel {
	Q_OBJECT

 public:
	explicit TreeModel(QObject* parent = nullptr);
	~TreeModel() override { delete rootItem; }

	[[nodiscard]] QVariant data(const QModelIndex& index,
	                            int role) const override;
	[[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

	// until clang-tidy 14 there is no way to unlint several lines at once :(
	// NOLINTNEXTLINE(google-default-arguments)
	[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
	                                  int role = Qt::DisplayRole) const override;
	// NOLINTNEXTLINE(google-default-arguments)
	[[nodiscard]] QModelIndex index(
			int row, int column,
			const QModelIndex& parent = QModelIndex()) const override;
	// NOLINTNEXTLINE(google-default-arguments)
	[[nodiscard]] QModelIndex parent(const QModelIndex& index) const override;
	// NOLINTNEXTLINE(google-default-arguments)
	[[nodiscard]] int rowCount(
			const QModelIndex& parent = QModelIndex()) const override;
	// NOLINTNEXTLINE(google-default-arguments)
	[[nodiscard]] int columnCount(
			const QModelIndex& parent = QModelIndex()) const override;

	void Fill(const InstanceList& data);

	void TestFillWithTxtFile(const QString& file_path);

 private:
	TreeItem* rootItem;
};
